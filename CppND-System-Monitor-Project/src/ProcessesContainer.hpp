#include "Process.hpp"
#include <string>
#include <vector>
using std::string;
using std::vector;

class ProcessesContainer {
public:
    ProcessesContainer() { this->refreshList(); }
    void refreshList();
    string printList();
    vector<string> getList();

private:
    vector<Process> _list;
};

void ProcessesContainer::refreshList() {
    vector<string> pList = ProcessParser::getPidList();
    this->_list.clear();
    for(auto pid: pList){
        Process proc(pid);
        this->_list.push_back(proc);
    }
}

string ProcessesContainer::printList() {
    string output;
    for(auto p: _list){ output += p.getProcess(); }
    return output;
}

vector<string> ProcessesContainer::getList() {
    vector<string> list;
    int start = ((this->_list.size()) > 20) ? this->_list.size() - 10 : 0;
    for(int i = start; i < (this->_list.size()); ++i) {
        list.push_back( this->_list[i].getProcess());
    }
    return list;
}