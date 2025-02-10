#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

long long gcd(long long a, long long b) {
    while (b != 0) {
        long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

long long modMultiply(long long a, long long b, long long mod) {
    long long result = 0;
    a = a % mod;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return result;
}

long long modPow(long long base, long long exponent, long long mod) {
    if (mod == 1) return 0;
    long long result = 1;
    base = base % mod;
    while (exponent > 0) {
        if (exponent % 2 == 1)
            result = modMultiply(result, base, mod);
        base = modMultiply(base, base, mod);
        exponent = exponent >> 1;
    }
    return result;
}

long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1)
        return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0)
        x1 += m0;
    return x1;
}

int letterToNumber(char c) {
    return toupper(c) - 'A';
}

char numberToLetter(int num) {
    return static_cast<char>((num % 26) + 'A');
}

vector<long long> textToBlocks(const string& text) {
    vector<long long> blocks;
    cout << "\nConverting text to number blocks:\n";
    
    for (size_t i = 0; i < text.length(); i += 2) {
        long long block = 0;
        if (isalpha(text[i])) {
            block = letterToNumber(text[i]) * 100;
            
            if (i + 1 < text.length() && isalpha(text[i + 1])) {
                block += letterToNumber(text[i + 1]);
            }
            
            cout << "'" << (char)toupper(text[i]);
            if (i + 1 < text.length() && isalpha(text[i + 1])) {
                cout << (char)toupper(text[i + 1]);
            }
            cout << "' -> " << block << endl;
            
            blocks.push_back(block);
        }
    }
    return blocks;
}

string blocksToText(const vector<long long>& blocks) {
    string text;
    for (long long block : blocks) {
        int first = block / 100;
        int second = block % 100;
        text += numberToLetter(first);
        if (second < 26) {
            text += numberToLetter(second);
        }
    }
    return text;
}

vector<long long> encryptWithSteps(const vector<long long>& numbers, long long e, long long n) {
    vector<long long> encrypted;
    cout << "\nEncryption steps:\n";
    cout << "Using public key (e, n): (" << e << ", " << n << ")\n";
    
    for (size_t i = 0; i < numbers.size(); i++) {
        cout << "\nEncrypting block " << numbers[i] << ":\n";
        cout << "C = M^e mod n\n";
        cout << "C = " << numbers[i] << "^" << e << " mod " << n << endl;
        
        long long result = modPow(numbers[i], e, n);
        cout << "C = " << result << endl;
        encrypted.push_back(result);
    }
    return encrypted;
}

vector<long long> decryptWithSteps(const vector<long long>& numbers, long long d, long long n) {
    vector<long long> decrypted;
    cout << "\nDecryption Process:\n";
    cout << "Using private key (d, n): (" << d << ", " << n << ")\n";
    cout << "Formula: M = C^d mod n\n\n";

    for (size_t i = 0; i < numbers.size(); i++) {
        cout << "Step " << i + 1 << ":\n";
        cout << "Decrypting ciphertext: " << numbers[i] << endl;
        cout << "M = " << numbers[i] << "^" << d << " mod " << n << endl;
        
        long long result = modPow(numbers[i], d, n);
        cout << "M = " << result << endl;
        
        cout << "Block " << result << " represents: ";
        int first = result / 100;
        int second = result % 100;
        cout << numberToLetter(first);
        if (second < 26) {
            cout << numberToLetter(second);
        }
        cout << endl << "-----------------------------\n";
        
        decrypted.push_back(result);
    }
    
    return decrypted;
}

int main() {
    string text;
    long long p, q, n, e, d;
    char mainChoice, keyChoice;
    vector<long long> numbers;

    while (true) {
        cout << "\nRSA Encryption/Decryption Step-by-Step\n";
        cout << "1. Encrypt\n";
        cout << "2. Decrypt\n";
        cout << "3. Exit\n";
        cout << "Enter choice (1-3): ";
        cin >> mainChoice;

        if (mainChoice == '3') {
            cout << "Goodbye!\n";
            break;
        }

        if (mainChoice != '1' && mainChoice != '2') {
            cout << "Invalid choice!\n";
            continue;
        }

        cout << "\nKey Input Options:\n";
        cout << "1. Enter p, q, and e/d\n";
        cout << "2. Enter n and e/d\n";
        cout << "Enter choice (1-2): ";
        cin >> keyChoice;

        if (keyChoice == '1') {
            cout << "\nEnter first prime number (p): ";
            cin >> p;
            cout << "Enter second prime number (q): ";
            cin >> q;
            n = p * q;
            long long phi = (p - 1) * (q - 1);

            cout << "\nStep 1: Calculate n = p * q\n";
            cout << "n = " << p << " * " << q << " = " << n << endl;
            
            cout << "\nStep 2: Calculate φ(n) = (p-1)(q-1)\n";
            cout << "φ(n) = (" << p << "-1)(" << q << "-1) = " << phi << endl;

            if (mainChoice == '1') {
                cout << "\nEnter public exponent (e): ";
                cout << "\n(1 < e < " << phi << " and gcd(e, " << phi << ") = 1): ";
                cin >> e;
                while (gcd(e, phi) != 1) {
                    cout << "Invalid e. Please enter a valid value for e: ";
                    cin >> e;
                }
                d = modInverse(e, phi);
                cout << "\nPrivate key d has been calculated: " << d << endl;
            } else {
                cout << "\nEnter private exponent (d): ";
                cin >> d;
                e = modInverse(d, phi);
                cout << "\nPublic key e has been calculated: " << e << endl;
            }
        }
        else if (keyChoice == '2') {
            cout << "\nEnter modulus (n): ";
            cin >> n;
            if (mainChoice == '1') {
                cout << "Enter public exponent (e): ";
                cin >> e;
            } else {
                cout << "Enter private exponent (d): ";
                cin >> d;
            }
        }
        else {
            cout << "Invalid choice!\n";
            continue;
        }

        if (mainChoice == '1') {
            cout << "\nEnter text to encrypt (letters only): ";
            cin.ignore();
            getline(cin, text);
            numbers = textToBlocks(text);
            vector<long long> encrypted = encryptWithSteps(numbers, e, n);
            
            cout << "\nFinal encrypted values:\n";
            for (size_t i = 0; i < encrypted.size(); i++) {
                cout << "Block " << i + 1 << ": " << encrypted[i] << endl;
            }
        } 
        else {
            cout << "\nEnter number of encrypted blocks: ";
            int count;
            cin >> count;
            cout << "\nEnter each encrypted value:\n";
            numbers.clear();
            for (int i = 0; i < count; i++) {
                cout << "Block " << i + 1 << ": ";
                long long val;
                cin >> val;
                numbers.push_back(val);
            }
            vector<long long> decrypted = decryptWithSteps(numbers, d, n);
            cout << "\nFinal decrypted text: " << blocksToText(decrypted) << endl;
        }
    }
    return 0;
}