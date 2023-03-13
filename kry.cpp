#include "kry.h"

int main(int argc, char **argv) {
    int eF, dF, cF, aF, bF, fF, oF, a, b, c;
    eF = dF = cF = aF = bF = fF = oF = a = b = 0;
    std::string inPath, outPath;
    char *cvalue, *ptr;
    
    while ((c = getopt(argc, argv, "edca:b:f:o:")) != -1)
    switch (c) {
        case 'e':
            eF = 1;
            break;
        case 'd':
            dF = 1;
            break;
        case 'c':
            cF = 1;
            break;
        case 'a':
            aF = 1;
            cvalue = optarg;
            a = strtol(cvalue, &ptr, 10);
            break;
        case 'b':
            bF = 1;
            cvalue = optarg;
            b = strtol(cvalue, &ptr, 10);
            break;
        case 'f':
            fF = 1;
            inPath = optarg;
            break;
        case 'o':
            oF = 1;
            outPath = optarg;
            break;
        case '?':
            fprintf(stderr, "Invalid options.");
            return 1;
        default:
            abort();
    }

    if ((eF + dF + cF != 1) || (eF && (aF + bF != 2)) ||
        (dF && (aF + bF != 2)) || (cF && (fF + oF != 2))) {
            std::cout << cF << std::endl;
            fprintf(stderr, "Invalid option combination.");
            return 1;
        }

    if (!cF && !checkA(a)) {
        fprintf(stderr, "Invalid value of 'a'.");
        return 1;
    }

    if (cF != 1 && optind == argc) {
        fprintf(stderr, "Missing input.");
        return 1;
    }

    std::string inputText = "";
    if (fF) {
        std::ifstream fileIn;
        fileIn.open(inPath);
        std::stringstream txt;
        txt << fileIn.rdbuf();
        inputText = txt.str();
        fileIn.close();
    } else {
        for (int index = optind; index < argc; index++) {
            inputText += argv[index];
        }
    }
    std::string result;
    if (eF) {
        result = encrypt(inputText, a, b);
        std::cout << result << std::endl;
    }
    if (dF) {
        result = decrypt(inputText, a, b);
        std::cout << result << std::endl;
    }
    if (cF) {
        result = decryptWithoutKey(inputText);
        std::ofstream fileOut;
        fileOut.open(outPath);
        fileOut << result;
        fileOut.close();
    }

    return 0;
}

bool checkA(int a) {
    int possibleA[] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    if (std::find(std::begin(possibleA), std::end(possibleA), a) == std::end(possibleA)) {
        return false;
    }
    return true;
}

int multInv(int a) {
    // calculation of the multiplicative inverse
    //based on the pseudocode from Nechvatal, J.: PUBLIC-KEY CRYPTOGRAPHY
    int g, h, w, z, v, r, temp, y;
    g = 26; h = a; w = 1; z = 0; v = 0; r = 1;
    while (h > 0) {
        y = g / h;
        temp = h; h = g - y * temp; g = temp;
        temp = z; z = w - y * temp; w = temp;
        temp = r; r = v - y *  temp; v = temp;
    }
    return v;
}

bool checkText(std::string text) {
    std::string czechWords[] = {"JSEM", "JAKO", "KDYZ", "JEHO", "BYLO", "BYLA", "REKL", "JESTE", "JSME", "KTERY", "NEBO", "KTERE", "PRED", "NECO", "TOHO", "BUDE", "JSTE", "JSOU"};
    for (auto &word : czechWords) {
        if (text.find(word) != std::string::npos)
            return true;
    }
    return false;
}

std::pair<int, int> calculateKeys(int max1, int max2, int max3) {
    int a = max3 - max1;
    int b = 0;
    if (((a % 2) + 2) % 2 == 0) {
        a = a / 2 * 21; // mult. inv. of 5
        a = ((a % 26) + 26) % 26;
        b = max2;
        return {a, b};
    } else {
        return {0, 0};
    }
}

std::string encrypt(std::string text, int a, int b) {
    std::string ctext = "";
    for (char &letter : text) {
        if (letter == ' ') {
            ctext += ' ';
        } else if (letter >= 65 && letter <= 90) {
            int newChar = ((a * (letter - 65) + b) % 26) + 65;
            ctext += char(newChar);
        } else if (letter >= 97 && letter <= 122) {
            int newChar = ((a * (letter - 32 - 65) + b) % 26) + 65;
            ctext += char(newChar);
        }
    }
    return ctext;
}

std::string decrypt(std::string text, int a, int b) {
    int inv = multInv(a);
    std::string msg = "";
    for (char &letter : text) {
        if (letter == ' ') {
            msg += ' ';
        } else if (letter >= 65 && letter <= 90) {
            // substract and add 65 to calculate from ASCII
            // (a % b + b) % b to correct the negative remainder
            int newChar = ((inv * (letter - 65 - b)) % 26 + 26) % 26 + 65;
            msg += char(newChar);
        }
    }
    return msg;
}

std::string decryptWithoutKey(std::string text) {
    int quantities[26] = {0};
    for (unsigned int i = 0; i < text.length(); i++) {
        if (text[i] != ' ') {
            quantities[text[i] - 65]++;
        }
    }

    int max1, max2, max3, maxI1, maxI2, maxI3;
    max1 = max2 = max3 = maxI1 = maxI2 = maxI3 = 0;
    for (int j = 0; j < 26; j++) {
        if (quantities[j] > max1) {
            max3 = max2; maxI3 = maxI2;
            max2 = max1; maxI2 = maxI1;
            max1 = quantities[j]; maxI1 = j;
        } else if (quantities[j] > max2) {
            max3 = max2; maxI3 = maxI2;
            max2 = quantities[j]; maxI2 = j;
        } else if (quantities[j] > max3) {
            max3 = quantities[j]; maxI3 = j;
        }
    }

    std::string output;
    std::pair<int, int> keys;
    keys = calculateKeys(maxI1, maxI2, maxI3);
    if (checkA(keys.first)) {
        output = decrypt(text, keys.first, keys.second);
        if (checkText(output)) {
            std::cout << "a=" << keys.first << ",b=" << keys.second << std::endl;
            return output;
        }
    }
    keys = calculateKeys(maxI2, maxI1, maxI3);
    if (checkA(keys.first)) {
        output = decrypt(text, keys.first, keys.second);
        if (checkText(output)) {
            std::cout << "a=" << keys.first << ",b=" << keys.second << std::endl;
            return output;
        }
    }
    keys = calculateKeys(maxI1, maxI3, maxI2);
    if (checkA(keys.first)) {
        output = decrypt(text, keys.first, keys.second);
        if (checkText(output)) {
            std::cout << "a=" << keys.first << ",b=" << keys.second << std::endl;
            return output;
        }
    }
    return "";
}