//HW8Q1 adaptive hybrid sorting(introsort lite)
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>

using namespace std;

int cmpQ = 0, swpQ = 0;
int cmpH = 0, swpH = 0;
int cmpI = 0, swpI = 0;

void insertionSort(vector<int>& a, int l, int r) {
    for (int i = l + 1; i <= r; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= l && (++cmpI, a[j] > key)) {
            a[j + 1] = a[j];
            --j;
            ++swpI;
        }
        a[j + 1] = key;
    }
}

void heapify(vector<int>& a, int l, int n, int i) {
    int largest = i;
    int L = 2 * i + 1, R = 2 * i + 2;
    if (L < n && (++cmpH, a[l + L] > a[l + largest])) largest = L;
    if (R < n && (++cmpH, a[l + R] > a[l + largest])) largest = R;
    if (largest != i) {
        swap(a[l + i], a[l + largest]);
        ++swpH;
        heapify(a, l, n, largest);
    }
}

void heapSort(vector<int>& a, int l, int r) {
    int n = r - l + 1;
    for (int i = n/2 - 1; i >= 0; --i)
        heapify(a, l, n, i);
    for (int i = n - 1; i > 0; --i) {
        swap(a[l], a[l + i]); ++swpH;
        heapify(a, l, i, 0);
    }
}

void intro(vector<int>& a, int l, int r, int depth) {
    if (r - l + 1 <= 16) {
        insertionSort(a, l, r);
        return;
    }
    if (depth == 0) {
        heapSort(a, l, r);
        return;
    }

    int pivot = a[r], i = l - 1;
    for (int j = l; j < r; ++j) {
        ++cmpQ;
        if (a[j] < pivot) {
            ++i; swap(a[i], a[j]); ++swpQ;
        }
    }
    swap(a[i + 1], a[r]); ++swpQ;
    int pi = i + 1;
    intro(a, l, pi - 1, depth - 1);
    intro(a, pi + 1, r, depth - 1);
}

void introSort(vector<int>& a) {
    int depth = 2 * log2(a.size());
    intro(a, 0, a.size() - 1, depth);
}

void reset() {
    cmpQ = swpQ = cmpH = swpH = cmpI = swpI = 0;
}

int main() {
    vector<int> sizes = {100, 1000, 10000};
    for (int s : sizes) {
        long long qc = 0, qs = 0, hc = 0, hs = 0, ic = 0, is = 0;
        for (int t = 0; t < 30; ++t) {
            vector<int> arr(s);
            generate(arr.begin(), arr.end(), rand);
            reset();
            introSort(arr);
            qc += cmpQ; qs += swpQ;
            hc += cmpH; hs += swpH;
            ic += cmpI; is += swpI;
        }
        cout << "Size: " << s << "\n";
        cout << "AlgPhase\tAvgCmp\tAvgSwp\n";
        cout << "Quick\t\t" << qc / 30 << "\t" << qs / 30 << "\n";
        cout << "Heap\t\t"  << hc / 30 << "\t" << hs / 30 << "\n";
        cout << "Insert\t\t" << ic / 30 << "\t" << is / 30 << "\n\n";
    }
    return 0;
}


//HW8Q2 Visual Trace of Insertion Sort
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

int main() {
    string line;
    vector<int> a;

    cout<<"SPACE TUSUNU KULLANARAK SAYI GIRIN(MAX20),BITINCE ENTERA BAS"<<endl;
    // Tek satır giriş al
    getline(cin, line);
    stringstream ss(line);
    int num;
    while (ss >> num && a.size() < 20) {
        a.push_back(num);
    }

    int swaps = 0, comps = 0;

    // Insertion Sort (trace)
    for (int i = 1; i < a.size(); ++i) {
        int j = i;
        while (j > 0 && (++comps, a[j - 1] > a[j])) {
            swap(a[j], a[j - 1]);
            ++swaps;
            cout << "Swap " << swaps << ": ";
            for (int v : a) cout << v << " ";
            cout << endl;
            --j;
        }
    }

    cout << "Total comparisons: " << comps << " | Total swaps: " << swaps << endl;
    return 0;
}

//HW8Q3 BFS and DFS path-listing utility
#include <iostream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_set>
#include <algorithm>
using namespace std;


unordered_map<char, vector<char>> graph;


vector<char> bfs(char start, char goal) {
    unordered_map<char, char> parent;
    unordered_set<char> visited;
    queue<char> q;

    q.push(start);
    visited.insert(start);
    parent[start] = '\0';

    while (!q.empty()) {
        char node = q.front(); q.pop();
        if (node == goal) break;

        for (char neighbor : graph[node]) {
            if (!visited.count(neighbor)) {
                visited.insert(neighbor);
                parent[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    vector<char> path;
    if (!parent.count(goal)) return path;
    for (char at = goal; at != '\0'; at = parent[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}


vector<char> dfs(char start, char goal) {
    unordered_map<char, char> parent;
    unordered_set<char> visited;
    stack<char> s;

    s.push(start);
    parent[start] = '\0';

    while (!s.empty()) {
        char node = s.top(); s.pop();
        if (visited.count(node)) continue;
        visited.insert(node);
        if (node == goal) break;

        for (auto it = graph[node].rbegin(); it != graph[node].rend(); ++it) {
            if (!visited.count(*it)) {
                s.push(*it);
                if (!parent.count(*it)) parent[*it] = node;
            }
        }
    }

    vector<char> path;
    if (!parent.count(goal)) return path;
    for (char at = goal; at != '\0'; at = parent[at])
        path.push_back(at);
    reverse(path.begin(), path.end());
    return path;
}


void printPath(string label, const vector<char>& path) {
    cout << label << ": ";
    for (char node : path)
        cout << node << " ";
    cout << "  (length " << (path.size() - 1) << ")" << endl;
}

int main() {

    graph['S'] = {'A', 'B'};
    graph['A'] = {'C'};
    graph['B'] = {'D'};
    graph['C'] = {'G'};
    graph['D'] = {'E'};
    graph['E'] = {'F'};
    graph['F'] = {'G'};
    graph['G'] = {};

    char start, goal;
    cout << "Start node: "; cin >> start;
    cout << "Goal node: "; cin >> goal;

  
    auto bfsPath = bfs(start, goal);
    auto dfsPath = dfs(start, goal);


    printPath("BFS", bfsPath);
    printPath("DFS", dfsPath);


    if (bfsPath.size() < dfsPath.size())
        cout << "Result: BFS is optimal because it found the shallowest goal first." << endl;
    else if (dfsPath.size() < bfsPath.size())
        cout << "Result: DFS is optimal in this case (shorter path found)." << endl;
    else
        cout << "Result: Both algorithms found paths of the same length." << endl;

    return 0;
}

//HW7Q1 increment via pointer
#include <iostream>
using namespace std;

// p'nin gösterdiği tamsayıyı 1 artırır
void increment(int *p) {
    *p = *p + 1;  // Dereference: p'nin gösterdiği değeri değiştiriyoruz
}

int main() {
    int x = 10;
    cout << "Initially x = " << x << endl;

    increment(&x);  // Address-of: x'in adresini gönderiyoruz

    cout << "After function call, x = " << x << endl;
    return 0;
}

//HW7Q2 Average Calculation Using a Dynamic Array
#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;

    // Dinamik double dizisi oluşturuluyor
    double* arr = new double[n];

    cout << "Enter " << n << " values: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    double average = sum / n;
    cout << "Average: " << average << endl;

    // Bellek serbest bırakılıyor
    delete[] arr;

    return 0;
}

//HW7Q3 Instertion and Deletion in a Dynamic array
#include <iostream>
using namespace std;

// Değer ekleme fonksiyonu
int* addEntry(int* arr, int &size, int value) {
    int* newArr = new int[size + 1];  // Yeni boyutlu dizi oluştur
    for (int i = 0; i < size; i++) {
        newArr[i] = arr[i];           // Eski elemanları kopyala
    }
    newArr[size] = value;             // Yeni değeri sona ekle
    size++;                           // Boyutu artır
    delete[] arr;                     // Eski diziyi sil
    return newArr;
}

// Eleman silme fonksiyonu
int* deleteEntry(int* arr, int &size, int index) {
    if (index < 0 || index >= size) {
        return arr; // Hatalı indeks -> dizi değişmeden döner
    }

    int* newArr = new int[size - 1]; // Yeni boyutlu dizi oluştur
    for (int i = 0, j = 0; i < size; i++) {
        if (i != index) {
            newArr[j++] = arr[i];    // index dışındakileri kopyala
        }
    }
    size--;                          // Boyutu azalt
    delete[] arr;                    // Eski diziyi sil
    return newArr;
}

int main() {
    int size = 5;
    int* arr = new int[size]{1, 2, 3, 4, 5};

    cout << "Initial array: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    // 10 ekle
    arr = addEntry(arr, size, 10);
    cout << "After addEntry(10): ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    // 2. index'teki elemanı sil
    arr = deleteEntry(arr, size, 2);
    cout << "After deleteEntry(index=2): ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << endl;

    delete[] arr;  // Belleği serbest bırak
    return 0;
}


//HW7Q4 Emulating a 2D array with a 1D dynamic Array
#include <iostream>
#include <cstdlib>  // exit için
using namespace std;


int* create2DArray(int rows, int cols) {
    int* arr = new int[rows * cols];
    for (int i = 0; i < rows * cols; i++) {
        arr[i] = 0; // başlangıçta her hücreyi sıfırla
    }
    return arr;
}


void set(int* arr, int rows, int cols, int r, int c, int val) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        cout << "Index out of bounds in set(" << r << ", " << c << ")" << endl;
        exit(1);
    }
    arr[r * cols + c] = val;
}


int get(int* arr, int rows, int cols, int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        cout << "Index out of bounds in get(" << r << ", " << c << ")" << endl;
        exit(1);
    }
    return arr[r * cols + c];
}

int main() {
    int rows, cols;
    cout << "Enter number of rows: ";
    cin >> rows;
    cout << "Enter number of columns: ";
    cin >> cols;

    // Matris oluştur
    int* matrix = create2DArray(rows, cols);

    // Değer ata (sorudaki gibi)
    set(matrix, rows, cols, 0, 0, 5);
    set(matrix, rows, cols, 0, 2, 7);
    set(matrix, rows, cols, 1, 1, 3);

    // Matrisi yazdır
    cout << "Matrix:\n";
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            cout << get(matrix, rows, cols, r, c) << " ";
        }
        cout << endl;
    }

    delete[] matrix; // belleği boşalt
    return 0;
}

//HW6Q1 MENU DRIVEN ARRAY OPERATIONS
#include <iostream>
#include <climits>  // <-- Bunu ekle
using namespace std;


void reverseArray(int arr[], int n) {
    for (int i = 0; i < n / 2; i++) {
        int temp = arr[i];
        arr[i] = arr[n - 1 - i];
        arr[n - 1 - i] = temp;
    }
    cout << "Reversed array: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

int findSecondMax(const int arr[], int n) {
    int max = INT_MIN, second = INT_MIN;
    for (int i = 0; i < n; i++) {
        if (arr[i] > max) {
            second = max;
            max = arr[i];
        } else if (arr[i] != max && arr[i] > second) {
            second = arr[i];
        }
    }
    if (second == INT_MIN)
        return INT_MIN;
    return second;
}

void mergeSorted(const int A[], int m, const int B[], int n, int C[]) {
    int i = 0, j = 0, k = 0;
    while (i < m && j < n) {
        if (A[i] < B[j])
            C[k++] = A[i++];
        else
            C[k++] = B[j++];
    }
    while (i < m)
        C[k++] = A[i++];
    while (j < n)
        C[k++] = B[j++];
}

int main() {
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1) Reverse an array\n";
        cout << "2) Find the second largest element\n";
        cout << "3) Merge two sorted arrays\n";
        cout << "4) Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                int n;
                cout << "-- Reverse an array --\n";
                cout << "n: ";
                cin >> n;
                if (n < 1 || n > 100) {
                    cout << "Invalid input.\n";
                    break;
                }
                int arr[100];
                cout << "Array: ";
                for (int i = 0; i < n; i++)
                    cin >> arr[i];
                reverseArray(arr, n);
                break;
            }

            case 2: {
                int n;
                cout << "-- Second largest element --\n";
                cout << "n: ";
                cin >> n;
                if (n < 2 || n > 100) {
                    cout << "Invalid input.\n";
                    break;
                }
                int arr[100];
                cout << "Array: ";
                for (int i = 0; i < n; i++)
                    cin >> arr[i];
                int second = findSecondMax(arr, n);
                if (second == INT_MIN)
                    cout << "Second largest not found.\n";
                else
                    cout << "Second largest: " << second << endl;
                break;
            }

            case 3: {
                int m, n;
                cout << "-- Merge two sorted arrays --\n";
                cout << "m: ";
                cin >> m;
                cout << "n: ";
                cin >> n;
                if (m < 1 || m > 100 || n < 1 || n > 100) {
                    cout << "Invalid input.\n";
                    break;
                }
                int A[100], B[100], C[200];
                cout << "A: ";
                for (int i = 0; i < m; i++)
                    cin >> A[i];
                cout << "B: ";
                for (int i = 0; i < n; i++)
                    cin >> B[i];
                mergeSorted(A, m, B, n, C);
                cout << "Merged array: ";
                for (int i = 0; i < m + n; i++)
                    cout << C[i] << " ";
                cout << endl;
                break;
            }

            case 4:
                cout << "Program terminated.\n";
                break;

            default:
                cout << "Invalid choice\n";
                break;
        }
    } while (choice != 4);

    return 0;
}

//HW6Q2 ROW AND COLUMN SUMS OF A MATRIX
#include <iostream>
using namespace std;

const int MAX_SIZE = 50;

void computeRowSums(const int mat[][MAX_SIZE], int M, int N, int rowSums[]) {
    for (int i = 0; i < M; i++) {
        rowSums[i] = 0;
        for (int j = 0; j < N; j++) {
            rowSums[i] += mat[i][j];
        }
    }
}

void computeColSums(const int mat[][MAX_SIZE], int M, int N, int colSums[]) {
    for (int j = 0; j < N; j++) {
        colSums[j] = 0;
        for (int i = 0; i < M; i++) {
            colSums[j] += mat[i][j];
        }
    }
}

int main() {
    int M, N;
    int mat[MAX_SIZE][MAX_SIZE];
    int rowSums[MAX_SIZE], colSums[MAX_SIZE];

    // Giriş alma
    cout << "M = ";
    cin >> M;
    cout << "N = ";
    cin >> N;

    if (M < 1 || M > 50 || N < 1 || N > 50) {
        cout << "Boyutlar 1 ile 50 arasında olmalıdır.\n";
        return 1;
    }

    cout << "mat =\n";
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> mat[i][j];
        }
    }

    computeRowSums(mat, M, N, rowSums);
    computeColSums(mat, M, N, colSums);

    // Çıktılar
    cout << "\nRow sums:\n";
    for (int i = 0; i < M; i++) {
        cout << rowSums[i] << " ";
    }
    cout << "\n\nColumn sums:\n";
    for (int j = 0; j < N; j++) {
        cout << colSums[j] << " ";
    }
    cout << endl;

    return 0;
}


//HW6Q3 NON OVERLAPPING PATTERN COUNT
#include <iostream>
#include <string>
using namespace std;

int countNonOverlapping(const string& text, const string& pattern) {
    int count = 0;
    size_t pos = 0;

    while ((pos = text.find(pattern, pos)) != string::npos) {
        count++;
        pos += pattern.length(); // üst üste binmemesi için pattern uzunluğu kadar atla
    }

    return count;
}

int main() {
    string text, pattern;

    // Giriş alma
    cout << "Text: ";
    getline(cin, text);

    cout << "Pattern: ";
    getline(cin, pattern);

    // Hesaplama
    int result = countNonOverlapping(text, pattern);

    // Çıktı
    cout << "Occurrences: " << result << endl;

    return 0;
}

//HW6Q4 STRING ARRAY UTILITIES
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

// 1) Duplicate Removal
int removeDuplicates(string strings[], int n) {
    int newSize = 0;
    for (int i = 0; i < n; i++) {
        bool found = false;
        for (int j = 0; j < newSize; j++) {
            if (strings[i] == strings[j]) {
                found = true;
                break;
            }
        }
        if (!found) {
            strings[newSize++] = strings[i];
        }
    }
    return newSize;
}

// 2) Sort Strings (Lexicographical Order)
void sortStrings(string strings[], int n) {
    sort(strings, strings + n);
}

// 3) Longest Common Prefix
string longestCommonPrefix(const string strings[], int n) {
    if (n == 0) return "";
    string prefix = strings[0];
    for (int i = 1; i < n; i++) {
        int j = 0;
        while (j < prefix.length() && j < strings[i].length() && prefix[j] == strings[i][j]) {
            j++;
        }
        prefix = prefix.substr(0, j);
        if (prefix.empty()) break;
    }
    return prefix;
}

// 4) Palindrome Detection
bool isPalindrome(const string& s) {
    int l = 0, r = s.length() - 1;
    while (l < r) {
        if (s[l++] != s[r--])
            return false;
    }
    return true;
}

int findPalindromes(const string strings[], int n, int palIndices[]) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (isPalindrome(strings[i])) {
            palIndices[count++] = i;
        }
    }
    return count;
}

// Main program
int main() {
    int n;
    string strings[100];

    cin >> n;
    cin.ignore(); // To consume the newline after the integer input
    for (int i = 0; i < n; i++) {
        getline(cin, strings[i]);
    }

    // Step 1: Remove duplicates
    int uniqueCount = removeDuplicates(strings, n);

    // Step 2: Sort
    sortStrings(strings, uniqueCount);

    // Step 3: Print unique sorted words
    cout << "\nUnique sorted words (count = " << uniqueCount << "):\n";
    for (int i = 0; i < uniqueCount; i++) {
        cout << strings[i] << " ";
    }
    cout << endl;

    // Step 4: Longest common prefix
    string prefix = longestCommonPrefix(strings, uniqueCount);
    cout << "\nLongest common prefix: \"" << prefix << "\"\n";

    // Step 5: Palindromes
    int palIndices[100];
    int palCount = findPalindromes(strings, uniqueCount, palIndices);

    cout << "\nPalindrome count: " << palCount << endl;
    if (palCount > 0) {
        cout << "Palindrome indices: ";
        for (int i = 0; i < palCount; i++) {
            cout << palIndices[i] << " ";
        }
        cout << endl;
    }

    return 0;
}

