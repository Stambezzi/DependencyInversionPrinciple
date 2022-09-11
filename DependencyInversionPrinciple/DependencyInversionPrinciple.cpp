#include <iostream>
#include <vector>
#include <tuple>
#include <cstdio>
#include <fstream>
#include <string>

enum ERelations 
{
    eParent,
    eChild,
    eSibling
};

class CPerson
{
public:
    CPerson(const std::string& name) : fName(name) {}

    bool operator==(const CPerson& other) const { return fName == other.fName; }

    std::string GetName() const { return fName; }
private:
    std::string fName;
};

class IRelationshipsBrowser
{
public:
    virtual std::vector<CPerson> FindAllRelationshipsOf(CPerson whom, ERelations relation) = 0;
};

class Relationships : public IRelationshipsBrowser
{
public:
    Relationships(std::vector<std::tuple<CPerson, ERelations, CPerson>> relationships) : fRelationships(relationships) {};

    virtual std::vector<CPerson> FindAllRelationshipsOf(CPerson whom, ERelations rel) override
    {
        std::vector<CPerson> result;
        for (const auto& relation : fRelationships)
        {
            if (std::get<0>(relation) == whom && std::get<1>(relation) == rel)
            {
                result.push_back(std::get<2>(relation));
            }
        }

        return result;
    }
private:
    std::vector<std::tuple<CPerson, ERelations, CPerson>> fRelationships;
};

class Research
{
public:
    Research(IRelationshipsBrowser& browser) : fBrowser(browser) {}
    void PrintAllRelationshipsOf(CPerson whom, ERelations relation)
    {
        std::vector<CPerson> result = fBrowser.FindAllRelationshipsOf( whom, relation );
        if (result.size() == 0)
        {
            std::cout << "No relationships of this kind for " << whom.GetName() << std::endl;
        }
        else
        {
            std::string strRelation;
            if (relation == ERelations::eChild)
            {
                strRelation = "Parents";
            }
            else if (relation == ERelations::eParent)
            {
                strRelation = "Children";
            }
            else
            {
                strRelation = "Siblings";
            }

            std::cout << strRelation << " of " << whom.GetName() << " are:" << std::endl;
            for (const auto& person : result)
            {
                std::cout << person.GetName() << std::endl;
            }
        }
    }
private:
    IRelationshipsBrowser& fBrowser;
};

int main()
{
    CPerson ivan("Ivan");
    CPerson tanya("Tanya");

    CPerson moni("Moni");
    CPerson pesho("Pesho");

    std::vector<std::tuple<CPerson, ERelations, CPerson>> relationships;
    relationships.push_back(std::make_tuple(ivan, ERelations::eParent, moni));
    relationships.push_back(std::make_tuple(ivan, ERelations::eParent, pesho));
    relationships.push_back(std::make_tuple(tanya, ERelations::eParent, moni));
    relationships.push_back(std::make_tuple(tanya, ERelations::eParent, pesho));

    relationships.push_back(std::make_tuple(moni, ERelations::eChild, ivan));
    relationships.push_back(std::make_tuple(moni, ERelations::eChild, tanya));
    relationships.push_back(std::make_tuple(pesho, ERelations::eChild, ivan));
    relationships.push_back(std::make_tuple(pesho, ERelations::eChild, tanya));

    relationships.push_back(std::make_tuple(moni, ERelations::eSibling, pesho));
    relationships.push_back(std::make_tuple(pesho, ERelations::eSibling, moni));

    Relationships rels(relationships);

    Research research(rels);
    research.PrintAllRelationshipsOf(ivan, ERelations::eParent);
    std::cout << std::endl;
    research.PrintAllRelationshipsOf(moni, ERelations::eSibling);
}
