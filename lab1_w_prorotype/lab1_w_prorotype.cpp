#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

class Document {
public:
    string name;
    string content;

    Document(const string& name, const string& content)
        : name(name), content(content) {}

    virtual ~Document() = default;

    virtual shared_ptr<Document> clone() const = 0;

    void display() const {
        cout << "Document: " << name << "\nContent: " << content << "\n";
    }

    void saveToFile(const string& name) const {
        string filename = name + ".txt";
        ofstream file(filename);
        if (file.is_open()) {
            file << "Document: " << name << "\n";
            file << "Content: " << content << "\n";
            file.close();
            cout << "Document saved to " << filename << "\n";
        }
        else {
            cerr << "Error: Unable to save document.\n";
        }
    }
};

class Report : public Document {
public:
    Report(const string& name, const string& content)
        : Document(name, content) {}

    shared_ptr<Document> clone() const override {
        return make_shared<Report>(*this);
    }
};

class Article : public Document {
public:
    Article(const string& name, const string& content)
        : Document(name, content) {}

    shared_ptr<Document> clone() const override {
        return make_shared<Article>(*this);
    }
};

class Contract : public Document {
public:
    Contract(const string& name, const string& content)
        : Document(name, content) {}

    shared_ptr<Document> clone() const override {
        return make_shared<Contract>(*this);
    }
};

class DocumentManager {
private:
    map<string, shared_ptr<Document>> templates;

public:
    void loadTemplatesFromFiles() {
        filesystem::path templatesDir = "templates";
        if (!filesystem::exists(templatesDir)) {
            filesystem::create_directory(templatesDir);
        }

        for (const auto& entry : filesystem::directory_iterator(templatesDir)) {
            if (entry.is_regular_file()) {
                ifstream file(entry.path());
                if (file.is_open()) {
                    string name = entry.path().stem().string();
                    string content;
                    getline(file, content); 
                    getline(file, content);
                    addTemplate(name, make_shared<Report>(name, content));
                    file.close();
                }
            }
        }
    }

    void addTemplate(const string& type, const shared_ptr<Document>& document) {
        templates[type] = document;
    }

    shared_ptr<Document> createDocument(const string& type) {
        if (templates.find(type) != templates.end()) {
            return templates[type]->clone();
        }
        else {
            throw invalid_argument("Template for " + type + " not found.");
        }
    }

    vector<string> getAvailableTemplates() const {
        vector<string> types;
        for (const auto& pair : templates) {
            types.push_back(pair.first);
        }
        return types;
    }

    void saveTemplateToFile(const string& name, const string& content) {
        filesystem::path templatesDir = "templates";
        if (!filesystem::exists(templatesDir)) {
            filesystem::create_directory(templatesDir);
        }

        string filename = templatesDir.string() + "/" + name + ".txt";
        ofstream file(filename);
        if (file.is_open()) {
            file << "Document: " << name << "\n";
            file << "Content: " << content << "\n";
            file.close();
            cout << "Template saved to " << filename << "\n";
        }
        else {
            cerr << "Error: Unable to save template.\n";
        }
    }
};

int main() {
    DocumentManager manager;

    manager.loadTemplatesFromFiles();

    while (true) {
        cout << "\nAvailable commands:\n";
        cout << "1. List templates\n";
        cout << "2. Create document from template\n";
        cout << "3. Add new template\n";
        cout << "4. Exit\n";
        cout << "Enter command: ";
        int command;
        cin >> command;

        if (command == 1) {
            cout << "Available templates:\n";
            for (const auto& type : manager.getAvailableTemplates()) {
                cout << "- " << type << "\n";
            }
        }
        else if (command == 2) {
            cout << "Enter template name: ";
            string choice;
            cin >> choice;

            try {
                auto document = manager.createDocument(choice);
                cout << "\nNew document created:\n";
                document->display();

                cout << "\nDo you want to edit the content? (yes/no): ";
                string edit;
                cin >> edit;

                if (edit == "yes") {
                    cout << "Enter new content: ";
                    cin.ignore();
                    getline(cin, document->content);
                    cout << "\nUpdated document:\n";
                    document->display();
                }

                cout << "\nDo you want to save this document to a file? (yes/no): ";
                string save;
                cin >> save;

                if (save == "yes") {
                    string filename;
                    cout << "Enter filename: ";
                    cin >> filename;
                    document->saveToFile(filename);
                }
            }
            catch (const invalid_argument& e) {
                cerr << e.what() << "\n";
            }
        }
        else if (command == 3) {
            cout << "Enter template name: ";
            string name;
            cin >> name;

            cout << "Enter template content: ";
            cin.ignore();
            string content;
            getline(cin, content);

            manager.saveTemplateToFile(name, content);
            manager.addTemplate(name, make_shared<Report>(name, content));
            cout << "Template added successfully.\n";
        }
        else if (command == 4) {
            break;
        }
        else {
            cout << "Invalid command. Try again.\n";
        }
    }

    return 0;
}