#include "SearchHistory.h"
#include <sstream>

SearchHistory::SearchHistory() : frequentSearch(5), history(100), searchedContacts(100), historySize(0) { }

SearchHistory::SearchHistory(const SearchHistory& other)
{
    frequentSearch = other.frequentSearch;
    history = other.history;
    searchedContacts = other.searchedContacts;
    historySize = other.historySize;
}

bool SearchHistory::check_frequent_search(Contact& contact)
{
    // If the top5Size is less than 5, the new contact should always be added
    if (frequentSearch.size() < 5) {
        return true;
    }

    for (int i = 0; i < frequentSearch.size(); i++)
    {
        // If the contact's search count is greater than the search count of any of the top 5 frequent searches
        if (frequentSearch[i].get_contact().get_search_count() < contact.get_search_count())
        {
            // Return true to indicate that the contact should be added to the top 5 frequent searches
            return true;
        }
    }
    return false;
}

void SearchHistory::update_frequent_search(HistoryObject& h)
{
    if (frequentSearch.size() < 5)
        frequentSearch.append(h);
    else
    {
        // If the top5 list is full, remove the contact with the lowest search count: last element
        frequentSearch.remove(frequentSearch[frequentSearch.size() - 1]); // Last element in the list
        frequentSearch.append(h);
    }
    // Sort the frequent search list based on search count
    sort_list(frequentSearch, &h, HistoryObject::by_search_count);
}

void SearchHistory::add_search_item(HistoryObject& historyObject) {
    // Assuming history is an array of HistoryObject instances
    if (historySize < 100) {
        history.append(historyObject);
        // Add the contact to the searchedContacts list
        searchedContacts.append(historyObject.get_contact());
        historySize++;
        if (check_frequent_search(historyObject.get_contact()))
        {
            update_frequent_search(historyObject);
        }
        save_history("history.txt");
        save_top5("top5.txt");
    }
    else {
        throw ("Cannot add more.");
    }
}

List<HistoryObject> SearchHistory::get_search_history() {
    // reverse search history to show latest first
    List<HistoryObject> temp = history;
    sort_list(temp, new HistoryObject(), HistoryObject::greater_than); // Sort in descending order
    return temp;
}

List<Contact> SearchHistory::get_searched_contacts() {
    // reverse searched contacts to show latest first
    List<Contact> temp;
    for (int i = historySize - 1; i >= 0; i--) {
        temp.append(searchedContacts[i]);
    }
    return temp;
}

// Return list of top 5 frequent searches
List<HistoryObject> SearchHistory::get_top5() {
    return frequentSearch;
}

// Save records to file
void SearchHistory::save_history(string historyFile) {
    ofstream file;
    file.open(historyFile);
    if (file.is_open()) {
        sort_list(history, &history[0], HistoryObject::greater_than);
        for (int i = 0; i < historySize; i++) {
            file << history[i] << endl;
        }
        file.close();
    }
}

void SearchHistory::save_top5(string top5File) {
    ofstream file;
    file.open(top5File);
    if (file.is_open()) {
        for (int i = 0; i < frequentSearch.size(); i++) {
            file << frequentSearch[i] << endl;
        }
        file.close();
    }
}
