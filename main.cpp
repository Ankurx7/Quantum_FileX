
#include<bits/stdc++.h>
using namespace std;

class HuffmanNode {
public:
    char data;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char data, int freq) {
        this->data = data;
        this->freq = freq;
        left = nullptr;
        right = nullptr;
    }

    ~HuffmanNode() {
        delete left;
        delete right;
    }
};

class Compare {
public:
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

class HuffmanCoding {
private:

    HuffmanNode* root;
    unordered_map<char, string> huffmanCodes;

public:
       HuffmanCoding(){
          root = nullptr;
       }

       ~HuffmanCoding() {
          delete root;
       }
    void encodeHelper(HuffmanNode* root, string code) {
        if (root == nullptr)
            return;

        if (!root->left && !root->right) {
            huffmanCodes[root->data] = code;
        }

        encodeHelper(root->left, code + "0");
        encodeHelper(root->right, code + "1");
    }

    void buildHuffmanTree(const unordered_map<char, int>& freqMap) {
        priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

        for (auto& pair : freqMap) {
            minHeap.push(new HuffmanNode(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            HuffmanNode* left = minHeap.top();
            minHeap.pop();

            HuffmanNode* right = minHeap.top();
            minHeap.pop();

            int sumFreq = left->freq + right->freq;
            HuffmanNode* newNode = new HuffmanNode('$', sumFreq);
            newNode->left = left;
            newNode->right = right;

            minHeap.push(newNode);
        }

        root = minHeap.top();
        encodeHelper(root, "");
    }

    void decodeHelper(HuffmanNode* root, int& index, const string& encodedData, string& result) {
        
        if (root == nullptr)
            return;

        if (!root->left && !root->right) {
            result += root->data;
            return;
        }

        index++;

        if (encodedData[index] == '0')
            decodeHelper(root->left, index, encodedData, result);
        else
            decodeHelper(root->right, index, encodedData, result);
    }



    string compress(const string& data) {
        unordered_map<char, int> freqMap;

        for (char c : data) {
            freqMap[c]++;
        }

        buildHuffmanTree(freqMap);

        string encodedData;
        for (char c : data) {
            encodedData += huffmanCodes[c];
        }

        return encodedData;
    }

    string decompress(const string& encodedData) {
        if (root == nullptr) {
            return "";
        }

        string decodedData;
        int index = -1;
        while (index < (int)encodedData.size() - 2) {
            decodeHelper(root, index, encodedData, decodedData);
        }

        return decodedData;
    }
};

class FileSystemNode {
public:
    string name;
    bool isDirectory;
    unordered_map<string, FileSystemNode*> children;
    string content; // Used for files
    string permissions;
    time_t modificationDate;

    FileSystemNode(string name, bool isDirectory){
        this->name = name;
        this->isDirectory = isDirectory;
        content = "";
        modificationDate = time(0);
    }
    ~FileSystemNode() {
        for (auto& child : children) {
            delete child.second;
        }
    }
};

class FileSystem {
public:
    FileSystemNode* root;
    HuffmanCoding huffman;

    FileSystemNode* findNode(vector<string>& path) {
        FileSystemNode* current = root;
        for (const auto& part : path) {
            if (current->children.find(part) == current->children.end()) {
                return nullptr;
            }
            current = current->children[part];
        }
        return current;
    }
    FileSystemNode* findAndCreateDir(vector<string>& path) {
        FileSystemNode* current = root;
        for (const auto& part : path) {
            if (current->children.find(part) == current->children.end()) {
                FileSystemNode* newDir = new FileSystemNode(part, true);
                current->children[part] = newDir;
                current = newDir;
            } else {
                if(!current->children[part]->isDirectory) return nullptr;
                current = current->children[part];
            }
        }
        return current;
    }
   void splitPath(const string& path, vector<string>& result) {
       result.clear();
       size_t start = 0, end = 0;
    
       while (start < path.size() && path[start] == '/') {
           ++start;
       }
       while ((end = path.find('/', start)) != string::npos) {
          if (end != start) {
              result.push_back(path.substr(start, end - start));
           }
           start = end + 1;
        
          while (start < path.size() && path[start] == '/') {
            ++start;
          }
      }
      if (start < path.size()) {
         result.push_back(path.substr(start));
      }
}

    string compress(const string& content) {
        return huffman.compress(content);
    }

    string decompress(const string& content) {
        return huffman.decompress(content);
    }

    string encrypt(const string& content) {
         string encryptedContent = "";
         for (char ch : content) {
            encryptedContent += ch + 1;
          }
         return encryptedContent;
    }
    string decrypt(const string& content) {
        string decryptedContent = "";
        for(char ch : content) {
          decryptedContent += ch - 1;
        }  
        return decryptedContent;
    }

    // Custom date parsing function
    bool parseDateTime(const std::string& dateTimeStr, struct tm& tm) {
        std::istringstream ss(dateTimeStr);
        ss >> get_time(&tm, "%Y-%m-%d %H:%M:%S");
        return !ss.fail();
    }

    FileSystem() {
        root = new FileSystemNode("/", true);
    }

    ~FileSystem() {
        delete root;
    }

    void createFile(const string& path, const string& content) {
        vector<string> parts;
        splitPath(path, parts);
        string fileName = parts.back();
        parts.pop_back();

        FileSystemNode* dir = findNode(parts);
        if (!dir || !dir->isDirectory) {
            cout<<"Invalid directory path."<<endl;
            return;
        }

        if (dir->children.find(fileName) != dir->children.end()) {
            if(dir->children[fileName]->isDirectory){
                cout<<"Error : A directory already exists with this name"<<endl;
            }    
            else cout<<"File already exists."<<endl;
            return;
        }

        FileSystemNode* file = new FileSystemNode(fileName, false);
        file->content = compress(encrypt(content));
        dir->children[fileName] = file;
    }

    void deleteFile(const string& path) {
        vector<string> parts;
        splitPath(path, parts);
        string fileName = parts.back();
        parts.pop_back();

        FileSystemNode* dir = findNode(parts);
        if (!dir || !dir->isDirectory || dir->children.find(fileName) == dir->children.end()) {
            cout << "File not found." << endl;
            return;
        }

        delete dir->children[fileName];
        dir->children.erase(fileName);
    }

    void readFile(const string& path) {
        vector<string> parts;
        splitPath(path, parts);

        FileSystemNode* file = findNode(parts);
        if (!file || file->isDirectory) {
            cout << "File not found." << endl;
            return;
        }

        cout << decrypt(decompress(file->content)) << endl;
    }

    void createDirectory(const string& path) {
        vector<string> parts;
        splitPath(path, parts);
        string dirName = parts.back();
        parts.pop_back();

        FileSystemNode* dir = findAndCreateDir(parts);
        if (!dir || !dir->isDirectory) {
            cout << "Invalid directory path." << endl;
            return;
        }

        if (dir->children.find(dirName) != dir->children.end()) {
            if(!dir->children[dirName]->isDirectory){
                cout<< "A file already exists with this name" <<endl;
            }
            else  cout << "Directory already exists." << endl;
            return;
        }

        FileSystemNode* newDir = new FileSystemNode(dirName, true);
        dir->children[dirName] = newDir;
    }

    void deleteDirectory(const string& path) {
        vector<string> parts;
        splitPath(path, parts);
        string dirName = parts.back();
        parts.pop_back();

        FileSystemNode* dir = findNode(parts);
        if (!dir || !dir->isDirectory || dir->children.find(dirName) == dir->children.end()) {
            cout << "Directory not found." << endl;
            return;
        }

        delete dir->children[dirName];
        dir->children.erase(dirName);
    }

    void listDirectory(const string& path) {
        vector<string> parts;
        splitPath(path, parts);

        FileSystemNode* dir = findNode(parts);
        if (!dir || !dir->isDirectory) {
            cout << "Directory not found." << endl;
            return;
        }

        for (const auto& child : dir->children) {
            cout << (child.second->isDirectory ? "D" : "F") << " " << child.first << endl;
        }
    }

    void updateFileMetadata(const string& path, const string& metadata, const string& value) {
        vector<string> parts;
        splitPath(path, parts);

        FileSystemNode* file = findNode(parts);
        if (!file || file->isDirectory) {
            cout << "File not found." << endl;
            return;
        }

        if (metadata == "permissions") {
            file->permissions = value;
        } else if (metadata == "modificationDate") {
            struct tm tm;
            if (parseDateTime(value, tm)) {
                file->modificationDate = mktime(&tm);
            } else {
                cout << "Invalid date format." << endl;
            }
        }
    }

    void updateDirectoryMetadata(const string& path, const string& metadata, const string& value) {
        vector<string> parts;
        splitPath(path, parts);

        FileSystemNode* dir = findNode(parts);
        if (!dir || !dir->isDirectory) {
            cout << "Directory not found." << endl;
            return;
        }

        if (metadata == "permissions") {
            dir->permissions = value;
        }
    }

    void searchFile(const string& name, FileSystemNode* node, const string& path) {
        if (!node) return;

        if (!node->isDirectory && node->name == name) {
            cout << path << "/" << name << endl;
            return;
        }

        for (const auto& child : node->children) {
            searchFile(name, child.second, path + "/" + node->name);
        }
    }

    void searchDirectory(const string& name, FileSystemNode* node, const string& path) {
        if (!node) return;

        if (node->isDirectory && node->name == name) {
            cout << path << "/" << name << endl;
            return;
        }

        for (const auto& child : node->children) {
            searchDirectory(name, child.second, path + "/" + node->name);
        }
    }

    size_t calculateDirectorySize(FileSystemNode* node) {
        if (!node->isDirectory) {
            return node->content.size();
        }

        size_t totalSize = 0;
        for (const auto& child : node->children) {
            totalSize += calculateDirectorySize(child.second);
        }

        return totalSize;
    }

    void traverseDFS(FileSystemNode* node, const string& path) {
        if (!node) return;

        cout << path << "/" << node->name << endl;
        for (const auto& child : node->children) {
            traverseDFS(child.second, path + "/" + node->name);
        }
    }

    void traverseBFS(FileSystemNode* node, const string& path) {
        if (!node) return;

        queue<pair<FileSystemNode*, string>> q;
        q.push({ node, path });

        while (!q.empty()) {
            auto currentNode = q.front().first;
            auto currentPath = q.front().second;
            q.pop();

            cout << currentPath << "/" << currentNode->name << endl;

            for (const auto& child : currentNode->children) {
                q.push({ child.second, currentPath + "/" + currentNode->name });
            }
        }
    }
};

int main() {
    
    FileSystem fs;
    string path, content, metadata, value;
    string name, permissions;
    vector<string> parts;
    FileSystemNode* dir;
    int choice;
    cout <<"--------------*----------------"<<endl;
    do {
        cout <<" FILE SYSTEM MENU: "<<endl;
        cout <<" 1. Create File "<<endl;
        cout <<" 2. Delete File "<<endl;
        cout <<" 3. Read File "<<endl;
        cout <<" 4. Create Directory "<<endl;
        cout <<" 5. Delete Directory "<<endl;
        cout <<" 6. List Directory "<<endl;
        cout <<" 7. Update File Metadata "<<endl;
        cout <<" 8. Update Directory Metadata "<<endl;
        cout <<" 9. Search File "<<endl;
        cout <<" 10. Search Directory "<<endl;
        cout <<" 11. Calculate Directory Size "<<endl;;
        cout <<" 12. Traverse Directory (DFS) "<<endl;
        cout <<" 13. Traverse Directory (BFS) "<<endl;
        cout <<" 0. Exit "<<endl;
        cout <<" Enter your choice: ";
        
        cin >> choice;

        cout<<endl<<"------------------*--------------------"<<endl;

        switch (choice) {
            case 0:
                cout << "Exiting program.\n";
                 break;

            case 1:
                cout << "Enter file path: ";
                cin >> path;
                cin.ignore();
                cout << "Enter file content: ";
                getline(cin, content);
                fs.createFile(path, content);
                break;

            case 2:
                cout << "Enter file path: ";
                cin >> path;
                fs.deleteFile(path);
                break;

            case 3:
                cout << "Enter file path: ";
                cin >> path;
                fs.readFile(path);
                 break;

            case 4:
                cout << "Enter directory path: ";
                cin >> path;
                cin.ignore();
                fs.createDirectory(path);
                 break;

            case 5:
                cout << "Enter directory path: ";
                cin >> path;
                fs.deleteDirectory(path);
                 break;

            case 6:
                cout << "Enter directory path: ";
                cin >> path;
                fs.listDirectory(path);
                 break;

            case 7:
                cout << "Enter file path to update metadata: ";
                cin >> path;
                cout << "Enter metadata type (permissions/modificationDate): ";
                cin >> metadata;
                if (metadata == "permissions") {
                    cout << "Enter new permissions: ";
                    cin >> value;
                } else if (metadata == "modificationDate") {
                    cout << "Enter new modification date (YYYY-MM-DD HH:MM:SS): ";
                    cin.ignore();
                    getline(cin, value);
                }
                fs.updateFileMetadata(path, metadata, value);
                 break;

            case 8:
                cout << "Enter directory path to update metadata: ";
                cin >> path;
                cout << "Enter metadata type (permissions): ";
                cin >> metadata;
                if (metadata == "permissions") {
                    cout << "Enter new permissions: ";
                    cin >> value;
                }
                fs.updateDirectoryMetadata(path, metadata, value);
                 break;
                

            case 9:
                cout << "Enter file name to search: ";
                cin >> name;
                fs.searchFile(name, fs.root, "/");
                 break;

            case 10:
                cout << "Enter directory name to search: ";
                cin >> name;
                fs.searchDirectory(name, fs.root, "/");
                 break;

            case 11:
                cout << "Enter directory path: ";
                cin >> path;
                parts.clear();
                fs.splitPath(path, parts);
                dir = fs.findNode(parts);
                if (dir) {
                    cout << "Total size: " << fs.calculateDirectorySize(dir) << " bytes" << endl;
                } else {
                    cout << "Directory not found." << endl;
                }
                 break;

            case 12:
                cout << "Enter directory path: ";
                cin >> path;
                parts.clear();
                fs.splitPath(path, parts);
                dir = fs.findNode(parts);
                if (dir) {
                    fs.traverseDFS(dir, path);
                } else {
                    cout << "Directory not found." << endl;
                }
                 break;

            case 13:
                cout << "Enter directory path: ";
                cin >> path;
                parts.clear();
                fs.splitPath(path, parts);
                dir = fs.findNode(parts);
                if (dir) {
                    fs.traverseBFS(dir, path);
                } else {
                    cout << "Directory not found." << endl;
                }
                 break;


            default:
                cout << "Invalid choice. Please enter a valid option.\n";
        }

        cout<<endl<<"------------------*------------------"<<endl<<endl;

    } while (choice != 0);

    return 0;
}
