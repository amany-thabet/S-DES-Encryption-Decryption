#include <bits/stdc++.h>
using namespace std;

// ---------------- Permutation ----------------
vector<int> permute(const vector<int>& input, const vector<int>& pattern) {
    vector<int> result;
    for (int i : pattern)
        result.push_back(input[i - 1]);
    return result;
}

// ---------------- Left Shift ----------------
vector<int> leftShift(vector<int> half, int shift) {
    vector<int> shifted(half.begin() + shift, half.end());
    shifted.insert(shifted.end(), half.begin(), half.begin() + shift);
    return shifted;
}

// ---------------- XOR ----------------
vector<int> XOR(const vector<int>& a, const vector<int>& b) {
    vector<int> res;
    for (int i = 0; i < a.size(); i++)
        res.push_back(a[i] ^ b[i]);
    return res;
}

// ---------------- Print ----------------
void printVec(const vector<int>& v) {
    for (int bit : v) cout << bit;
    cout << endl;
}

// ---------------- S-Boxes ----------------
vector<int> S0(vector<int> bits) {
    int s0[4][4] = {
            {1,0,3,2},
            {3,2,1,0},
            {0,2,1,3},
            {3,1,3,2}
    };
    int row = bits[0]*2 + bits[3];
    int col = bits[1]*2 + bits[2];
    int val = s0[row][col];
    return { (val >> 1) & 1, val & 1 };
}

vector<int> S1(vector<int> bits) {
    int s1[4][4] = {
            {0,1,2,3},
            {2,0,1,3},
            {3,0,1,0},
            {2,1,0,3}
    };
    int row = bits[0]*2 + bits[3];
    int col = bits[1]*2 + bits[2];
    int val = s1[row][col];
    return { (val >> 1) & 1, val & 1 };
}

// ---------------- F Function ----------------
vector<int> F(vector<int> R, vector<int> K) {

    vector<int> EP = {4,1,2,3,2,3,4,1};
    vector<int> P4 = {2,4,3,1};

    vector<int> expanded = permute(R, EP);
    vector<int> xored = XOR(expanded, K);

    vector<int> left(xored.begin(), xored.begin() + 4);
    vector<int> right(xored.begin() + 4, xored.end());

    vector<int> s0 = S0(left);
    vector<int> s1 = S1(right);

    vector<int> combined;
    combined.insert(combined.end(), s0.begin(), s0.end());
    combined.insert(combined.end(), s1.begin(), s1.end());

    return permute(combined, P4);
}

// ---------------- Key Generation ----------------
void generateKeys(vector<int> key, vector<int>& K1, vector<int>& K2) {

    vector<int> P10 = {3,5,2,7,4,10,1,9,8,6};
    vector<int> P8  = {6,3,7,4,8,5,10,9};

    key = permute(key, P10);

    cout << "After P10: ";
    printVec(key);

    vector<int> L(key.begin(), key.begin() + 5);
    vector<int> R(key.begin() + 5, key.end());

    // LS-1
    L = leftShift(L, 1);
    R = leftShift(R, 1);

    cout << "After LS-1: ";
    printVec(L);
    printVec(R);

    vector<int> combined;
    combined.insert(combined.end(), L.begin(), L.end());
    combined.insert(combined.end(), R.begin(), R.end());

    K1 = permute(combined, P8);

    cout << "K1: ";
    printVec(K1);

    // LS-2
    L = leftShift(L, 2);
    R = leftShift(R, 2);

    cout << "After LS-2: ";
    printVec(L);
    printVec(R);

    combined.clear();
    combined.insert(combined.end(), L.begin(), L.end());
    combined.insert(combined.end(), R.begin(), R.end());

    K2 = permute(combined, P8);

    cout << "K2: ";
    printVec(K2);
}

// ---------------- S-DES (Encrypt / Decrypt) ----------------
vector<int> SDES(vector<int> data, vector<int> K1, vector<int> K2, bool encrypt) {

    vector<int> IP = {2,6,3,1,4,8,5,7};
    vector<int> IP_INV = {4,1,3,5,7,2,8,6};

    if (!encrypt) swap(K1, K2);

    data = permute(data, IP);

    vector<int> L(data.begin(), data.begin() + 4);
    vector<int> R(data.begin() + 4, data.end());

    // Round 1
    vector<int> temp = XOR(L, F(R, K1));
    L = R;
    R = temp;

    // Round 2
    temp = XOR(L, F(R, K2));
    L = temp;

    vector<int> combined;
    combined.insert(combined.end(), L.begin(), L.end());
    combined.insert(combined.end(), R.begin(), R.end());

    return permute(combined, IP_INV);
}

// ---------------- Main ----------------
int main() {

    // -------- Input Key --------
    string key_input;
    cout << "Enter 10-bit key: " << flush;
    cin >> key_input;

    vector<int> key;
    for (char c : key_input)
        key.push_back(c - '0');

    vector<int> K1, K2;
    generateKeys(key, K1, K2);

    // -------- Input Plaintext --------
    string pt_input;
    cout << "Enter 8-bit plaintext: " << flush;
    cin >> pt_input;

    vector<int> pt;
    for (char c : pt_input)
        pt.push_back(c - '0');

    // Encryption
    vector<int> cipher = SDES(pt, K1, K2, true);

    cout << "Cipher: ";
    printVec(cipher);

    // Decryption
    vector<int> decrypted = SDES(cipher, K1, K2, false);

    cout << "Decrypted: ";
    printVec(decrypted);

    return 0;
}




 
      

   

