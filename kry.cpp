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
        case 'o':
            oF = 1;
            outPath = optarg;
        case '?':
            fprintf(stderr, "Invalid options.");
            return 1;
        default:
            abort();
    }

    if ((eF + dF + cF != 1) || (eF && (aF + bF != 2)) ||
        (fF && (aF + bF != 2)) || (cF && (fF + oF != 2))) {
            fprintf(stderr, "Invalid options.");
            return 1;
        }

    int possibleA[] = {1, 3, 5, 7, 9, 11, 15, 17, 19, 21, 23, 25};
    if (std::find(std::begin(possibleA), std::end(possibleA), a) == std::end(possibleA)) {
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
    } else {
        for (int index = optind; index < argc; index++) {
            inputText += argv[index];
        }
    }
    std::string result;
    if (eF) result = encrypt(inputText, a, b);
    if (dF) result = decrypt(inputText, a, b);
    // if (cF) result = decryptWithoutKey(inputText);
    std::cout << result << std::endl;

    return 0;
}

std::string encrypt(std::string text, int a, int b) {
    std::string ctext = "";
    for (char &letter : text) {
        if (letter == ' ') {
            ctext += ' ';
        } else {
            int newChar = ((a * (letter - 65) + b) % 26) + 65;
            ctext += char(newChar);
        }
    }
    return ctext;
}

std::string decrypt(std::string text, int a, int b) {
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
    int inv = v % 26;
    std::string msg = "";
    for (char &letter : text) {
        if (letter == ' ') {
            msg += ' ';
        } else {
            // substract and add 65 to calculate from ASCII
            // (a % b + b) % b to correct the negative remainder
            int newChar = ((inv * (letter - 65 - b)) % 26 + 26) % 26 + 65;
            msg += char(newChar);
        }
    }
    return msg;
}

