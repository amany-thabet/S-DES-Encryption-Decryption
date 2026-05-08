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


/*
// دالة واحدة تقوم بالتشفير وفك التشفير معاً
string caesar(string text, int key) {
    string result = "";

    for (char c : text) {
        // التحقق أن الحرف من a إلى z
        if (c >= 'a' && c <= 'z') {
            // تطبيق القاعدة الرياضية: (pi + k) mod 26
            result += char((c - 'a' + key) % 26 + 'a');
        } else {
            // إضافة المسافات كما هي
            result += c;
        }
    }
    return result;
}



    return 0;
}
/*
string msg ,result;
    getline(cin,msg);

    int shift; cin>>shift;
    //encrypted
    string encrmsg="";
    for (int i=0;i<msg.length();i++) {
        char c = msg[i];

        if (isupper(c)) {
            char start_point = 'A'; // تعادل 65
            // تطبيق معادلة التشفير
            char encrypted_char = ((c - start_point + shift) % 26) + start_point;
            encrmsg += encrypted_char;
        } else if (islower(c)) {
            char start_point = 'a'; // تعادل 97
            // تطبيق معادلة التشفير
            char encrypted_char = ((c - start_point + shift) % 26) + start_point;
            encrmsg += encrypted_char;
        }

    else {
        // الرموز والمسافات والأرقام تنزل كما هي دون تغيير
        encrmsg+= c;
    }
}

cout<< "Encrypted: " << encrmsg << "\n";


string decrpmsg="";
    for (int i=0;i<encrmsg.length();i++) {
        char c = encrmsg[i];
        if (isalpha(c)) {
            if (isupper(c)) {
                char start_point = 'A'; // 65
                // تطبيق معادلة فك التشفير
                char original_char = ((c - start_point - shift + 26) % 26) + start_point;
                decrpmsg += original_char;
            } else if (islower(c)) {
                char start_point = 'a'; // 97
                // تطبيق معادلة فك التشفير
                char original_char = ((c - start_point - shift + 26) % 26) + start_point;
                decrpmsg += original_char;
            }
        }
        else {
                // الرموز والمسافات
                decrpmsg += c;
            }
        }

    cout << "Decrypted: " << decrpmsg << "\n";

-----------------------------------
/*
// دالة واحدة تقوم بالتشفير وفك التشفير معاً
string caesar(string text, int key) {
    string result = "";

    for (char c : text) {
        // التحقق أن الحرف من a إلى z
        if (c >= 'a' && c <= 'z') {
            // تطبيق القاعدة الرياضية: (pi + k) mod 26
            result += char((c - 'a' + key) % 26 + 'a');
        } else {
            // إضافة المسافات كما هي
            result += c;
        }
    }
    return result;
}

 / network paractical
    // المثال المذكور في المحاضرة
    string text = "meet me after the toga party";
    int key = 3; // مقدار الإزاحة

    // 1. التشفير
    string cipherText = caesar(text, key);
    cout << "Encrypted: " << cipherText << endl;

    // 2. فك التشفير (نستخدم نفس الدالة ولكن بمفتاح معكوس 26 - 3 = 23)
    string plainText = caesar(cipherText, 26 - key);
    cout << "Decrypted: " << plainText << endl;


------------------------------------------------------------
/*
 * int n; cin>>n;
        vector<string > v(n);
        int cnt=1;
        for (int i = 0; i <n; i++) {
            cin >> v[i];
        }
        for (int i = 1; i <n; i++) {
            if (v[i] != v[i - 1]) {
                cnt++;
            }
        }
        cout<<cnt;

 *  int flag=0;
    int arr[a]; int arr2[b];
    for (int i=0;i<a;i++){
        cin>>arr[i];
    }
    for (int i=0;i<b;i++){
        cin>>arr2[i];
    }
    for(int i=0;i<b;i++){
        if(arr[i]+arr[i+1]==arr2[i]){
          flag=1;
        }
        else if (arr2[i]+arr2[i+1]==arr[i]){
          flag=1;
        }
    }
  if(flag==1){
      cout<<"Yes";
  }
  else
      cout<<"No";

 * int n;
    cin>>n;
    int maxlen=0;
    vector<string> v(n);
    int x = 0;
      for (int i=0;i<n;i++) {
         cin>>v[i];
      }
        for (int i = 0; i <n ;i++) {
            if (v[i].size() > maxlen) {
                maxlen = v[i].size();
            }
        }
        for (int i = 0; i < n ; i++) {
            x = maxlen - v[i].size();


            for (int j = 0; j< x / 2; j++)cout << ".";
            cout << v[i];
            for (int j = 0; j< x / 2; j++)cout << ".";
            cout << "\n";
        }


 * -------------------
 *  while (t--) {
        int n;
        cin >> n;
        if (n<=3) {
            cout << -1 << endl;
        }
        for (int i=n;i>=1;i--){
            if (i%2==1){
                cout<< i <<" ";
                continue;
            }
        }
        for (int i=n;i>=1;i--){
            if (i%2==0){
                cout<< i <<" ";
            }
        }
        cout<<endl;


    }
 * -------------------------
 *  int t;
  cin>>t;
  while (t--) {
      int n, k;
      cin >> n >> k;
      ll x = ((k - 1) / (n - 1));
      cout << x + k << endl;
  }
 */
/*
    ll n,k;
    cin>>n>>k;
    ll x=(n-(k-1));
    ll y=n-2*(k-1);
    if( x>0 && x%2==1){
        cout<<"YES"<<endl;
        for (int i=0;i<k-1;i++)cout<< 1 <<" ";
        cout<< x <<endl;
        continue;
    }
    if( y>0 && y%2==0){
        cout<<"YES"<<endl;
        for (int i=0;i<k-1;i++) cout<< 2 <<" ";
        cout<< y << endl;
        continue;
    }
    cout<<"NO"<<endl;

}
//-----------------------------
/*
int t ; cin>>t;

while (t--) {

int n;
cin>>n;
ll p=1;
vector <int >arr;
while(n!=0){
if ( n%10>0){
arr.push_back( (n%10) * p);
}
n/=10;
p *=10;
}
cout<<arr.size()<<endl;
for (int i=0;i<arr.size();i++)
cout<<arr[i]<<" ";
cout<<endl;
}

/*----------------------------
 *   string a,b,x; cin>>a>>b;
        x[0] =a[0];
       a[0]=b[0];
       b[0]=x[0];
       cout <<a<<" "<<b;
       cout<<"\n";
 * --------------------------------
 * int t; cin>>t;
    while (t--) {
        int n, m, l, r;
        cin >> n >> m >> l >> r;
        if (m < r) {
            cout << 0 << " " << m << "\n";
        } else if (m > r) {
            cout << -(m - r) << " " << r << "\n";

        } else
            cout << (m / l) << " " << (m / r) << "\n";
    }
    -----------------------
    int t; cin>> t;
    while (t--) {
        int n;
        cin >> n;
        vector<vector<int>> v(n + 1, vector<int>(n + 1));
        vector<int> p(2*n + 1,0);
        ll sum=0;
        ll all = 1ll *(2 * n) * (2 * n + 1)/ 2;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cin >> v[i][j];
                if (p[i+j]==0){
                    p[i+j]=v[i][j];
                    sum+=v[i][j];
                }
            }

        }
          p[1]=all-sum;



        for (int i = 1; i <=2*n; i++) {
            cout << p[i] << " ";
        }
        cout<<"\n";
    }
 */