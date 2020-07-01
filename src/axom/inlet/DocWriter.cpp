#include "axom/inlet/DocWriter.hpp"
#include <iostream>
#include <assert.h>
#include "axom/slic.hpp"

namespace axom
{
namespace inlet
{
 
DocWriter::DocWriter(const std::string& fileName, axom::sidre::Group* root) {
  // assert(root);     // change to SLIC assert - slic is a logging lib
  SLIC_ASSERT_MSG(root != nullptr, "Sidre Group is null pointer");
  outFile.open(fileName);
  sidreGroupRoot = root;
  rstTable = {{"Field Name", "Description", "Default Value", "Range", "Required"}};
  writeDocuments(sidreGroupRoot);
  // TO DO: figure out how to get title -> potentially looking at datastore
  writeTable("title tbd");
}

void DocWriter::writeDocuments(axom::sidre::Group* sidreGroup) {
  SLIC_ASSERT_MSG(sidreGroup, "Root passed into writeDocuments shouldn't be nullptr");
  axom::sidre::IndexType i = sidreGroup->getFirstValidGroupIndex();
  if (sidreGroup != sidreGroupRoot && i == axom::sidre::InvalidIndex) {    // is root won't work because youre not necessarily wrking from the base
    // means that it is a field
    // so attributes are stored in views
    std::vector<std::string> fieldAttributes(5, "Not specified");
    fieldAttributes.resize(5);
    fieldAttributes[0] = sidreGroup->getName();
    if (sidreGroup->hasView("description")) {
      fieldAttributes[1] = std::string(sidreGroup->getView(sidreGroup->getViewIndex("description"))->getString());
    }
    // fieldAttributes[2] = std::to_string(sidreGroup->getView(sidreGroup->getViewIndex("default values"))->getScalar());
    // fieldAttributes[3] = std::to_string(sidreGroup->getView(sidreGroup->getViewIndex("range"))->getScalar());
    if (sidreGroup->hasView("required")) {
      int8 required = sidreGroup->getView(sidreGroup->getViewIndex("required"))->getData();
      fieldAttributes[4] = required ? "True" : "False";
    } 
  
    rstTable.push_back(fieldAttributes);
    // LEFT OFF HERE
  } 
  // axom::sidre::indexIsValid(i)
  while (i != axom::sidre::InvalidIndex) {
    // if (sidreGroup->getGroup(i)) {
    writeDocuments(sidreGroup->getGroup(i));
    // } else {
    //   std::cerr << "GROUP NOT FOUND ERROR?" << std::endl;
    //   if (!axom::sidre::indexIsValid(i)) {
    //     std::cerr<< "INDEX NOT VALID" << std::endl;
    //   } else {
    //     std::cerr << "index is valid" << std::endl;
    //   }
    // }

    i = sidreGroup->getNextValidGroupIndex(i);
  }
}

void DocWriter::writeTitle(const std::string& title) {
  assert(outFile.is_open());
  std::string stars;
  for (int i = 0; i < title.length(); i++) {
    stars += "*";
  }
  outFile << stars << "\n" << title << "\n" << stars << "\n";
}

void DocWriter::writeSubtitle(const std::string& sub) {
  assert(outFile.is_open());
  std::string pounds;
  for (int i = 0; i < sub.length(); i++) {
    pounds += "#";
  }
  outFile << pounds << "\n" << sub << "\n" << pounds << "\n";
}

void DocWriter::writeTable(const std::string& title) {
  assert(outFile.is_open());
  std::string result = ".. list-table:: " + title;
  result += "\n   :widths: 25 25 25 25 25\n   :header-rows: 1\n   :stub-columns: 1\n\n";
  // RSTtable = {{"Field Name", "Description", "Default Value", "Range", "Required"},
  // {"x", "number", "anything", "10", "true"}};
  // expect num cols = 5
  for (int i = 0; i < rstTable.size(); i++) {
    result += "   * - ";
    for (int j = 0; j < rstTable[i].size(); j++) {
      if (j != 0) {
        result += "     - ";
      }
      result += rstTable[i][j] + "\n";
    }
  }
  outFile << result;
}
}
}

// int main() {
//     std::ofstream file;
//     file.open("sample.rst");
//     if (!file.is_open()) {
//         std::cerr << "ERROR!!" << std::endl;
//     }
//     file << writeTitle("This is a title for the sample document");
//     file << writeSubtitle("This is a subtitle");
//     std::vector<std::vector<std::string>> v;
//     std::string str = writeTable("sample table is title", v);
//     file << str;
//     file.close();
//     return 0;
// }    