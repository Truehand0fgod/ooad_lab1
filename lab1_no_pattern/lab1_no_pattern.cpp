#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <fstream>

class Document {
public:
    std::string name;
    std::string content;
    Document() : name(""), content("") {}
    Document(const std::string& name, const std::string& content)
        : name(name), content(content) {}

    void display() const {
        std::cout << "Document: " << name << "\nContent: " << content << "\n";
    }

    void saveToFile(const std::string& name) const {
        std::string filename = name + ".txt";
        std::ofstream file(filename);
        if (file.is_open()) {
            file << "Document: " << name << "\n";
            file << "Content: " << content << "\n";
            file.close();
            std::cout << "Document saved to " << filename << "\n";
        }
        else {
            std::cerr << "Error: Unable to save document.\n";
        }
    }
    Document createCopy(const Document& original);
};
Document Document::createCopy(const Document& original) {
    return Document(original.name, original.content);
}

int main() {
    Document reportTemplate("Monthly Report", "This is a report template.");
    Document articleTemplate("Tech Article", "This is an article template.");
    Document contractTemplate("Service Contract", "This is a contract template.");

    std::map<std::string, Document> templates = {
        {"Report", reportTemplate},
        {"Article", articleTemplate},
        {"Contract", contractTemplate}
    };

    std::cout << "Available document templates:\n";
    for (const auto& temp : templates) {
        std::cout << "- " << temp.first << "\n";
    }

    while (true) {
        std::cout << "\nEnter the type of document to create (or 'exit' to quit): ";
        std::string choice;
        std::cin >> choice;

        if (choice == "exit") {
            break;
        }

        try {
            if (templates.find(choice) == templates.end()) {
                throw std::invalid_argument("Template for " + choice + " not found.");
            }

            Document document;
            document.createCopy(templates[choice]);
            std::cout << "\nNew document created:\n";
            document.display();

            std::cout << "\nDo you want to edit the content? (yes/no): ";
            std::string edit;
            std::cin >> edit;

            if (edit == "yes") {
                std::cout << "Enter new content: ";
                std::cin.ignore();
                std::getline(std::cin, document.content);
                std::cout << "\nUpdated document:\n";
                document.display();
            }

            std::cout << "\nDo you want to save this document to a file? (yes/no): ";
            std::string save;
            std::cin >> save;

            if (save == "yes") {
                std::cout << "Enter filename: ";
                std::string filename;
                std::cin >> filename;
                document.saveToFile(filename);
            }
        }
        catch (const std::invalid_argument& e) {
            std::cerr << e.what() << "\n";
        }
    }

    return 0;
}