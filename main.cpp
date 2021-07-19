// Example program
#include <ctime>
#include <iostream>
#include <set>
#include <sstream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class DateTime {
public:
  DateTime() {
    memset(&m_tm, 0, sizeof(m_tm));
    m_timestamp = 0;
  }

  void setDateTime(string format) {
    memset(&m_tm, 0, sizeof(m_tm));
    strptime(format.c_str(), "%m/%d/%Y", &m_tm);
    m_timestamp = mktime(&m_tm);
    m_dateStr = format;
  }

  int diffTIme(string dtime) {
    struct tm tmCurr;
    memset(&tmCurr, 0, sizeof(tmCurr));
    strptime(dtime.c_str(), "%m/%d/%Y", &tmCurr);

    time_t current = mktime(&tmCurr);

    if (m_timestamp != (time_t)(-1) && current != (time_t)(-1)) {
      double difference = difftime(current, m_timestamp) / (60 * 60 * 24);
      cout << std::ctime(&m_timestamp);
      cout << std::ctime(&current);
      cout << "difference = " << abs(difference) << " days" << endl;
      return abs(difference);
    }
    return 0;
  }

  string getDateStr() { return m_dateStr; }

private:
  struct tm m_tm;
  time_t m_timestamp;
  string m_dateStr;
};

class ITrade {

public:
  ITrade() : m_value(0) { m_clientSector = ""; }
  virtual ~ITrade() {}
  virtual double Value() = 0;
  virtual string ClientSector() = 0;
  virtual DateTime NextPaymentDate() = 0;
  virtual void classifier() = 0;

  void setClientSector(string str) { m_clientSector = str; }
  void setValue(double value) { m_value = value; }
  void setNextPaymentDate(DateTime dt) { m_nextPaymentDate = dt; }
  void setNextPaymentDate(string dtStr) {
    m_nextPaymentDate.setDateTime(dtStr);
  }

protected:
  double m_value;
  string m_clientSector;
  DateTime m_nextPaymentDate;
  DateTime m_referenceDate;
};

class TradePublic : public ITrade {

public:
  TradePublic(string refDate) { m_referenceDate.setDateTime(refDate); }
  ~TradePublic() {}
  double Value() { return m_value; }
  string ClientSector() { return m_clientSector; }
  DateTime NextPaymentDate() { return m_nextPaymentDate; }
  void classifier() {
    int diffdays = m_nextPaymentDate.diffTIme(m_referenceDate.getDateStr());
    if (diffdays > 30) {
      cout << "DEFAULTED";
    } else if (Value() > 1000000) {
      cout << "MEDIUMRISK";
    } else {
      cout << "HIGHRISK";
    }
    cout << " Classifier of TradePublic Class" << endl;
  }
};

class TradePrivate : public ITrade {

public:
  TradePrivate(string refDate) { m_referenceDate.setDateTime(refDate); }
  ~TradePrivate() {}
  double Value() { return m_value; }
  string ClientSector() { return m_clientSector; }
  DateTime NextPaymentDate() { return m_nextPaymentDate; }
  void classifier() {
    int diffdays = m_nextPaymentDate.diffTIme(m_referenceDate.getDateStr());
    if (diffdays > 30) {
      cout << "DEFAULTED";
    } else if (Value() > 1000000) {
      cout << "HIGHRISK";
    } else {
      cout << "MEDIUMRISK";
    }
    cout << " Classifier of TradePrivate Class" << endl;
  }
};

int main() {
  vector<ITrade *> entryList;
  string date;
  cout << "What is the reference date? ";
  getline(cin, date);

  string n;
  cout << "What is the number of trades? ";
  getline(cin, n);

  int count = 0;
  stringstream ss;
  ss << n;
  ss >> count;

  for (int i = 0; i < count; ++i) {
    ITrade *trade;

    string input;
    getline(cin, input);

    // Split readline
    vector<string> strs;
    size_t pos = input.find(' ');
    size_t initialPos = 0;
    while (pos != std::string::npos) {
      strs.push_back(input.substr(initialPos, pos - initialPos));
      initialPos = pos + 1;
      pos = input.find(' ', initialPos);
    }
    strs.push_back(
        input.substr(initialPos, min(pos, input.size()) - initialPos + 1));

    // Determine the Client Sector
    if (!strs[1].compare("Public")) {
      trade = new TradePublic(date);
    } else {
      trade = new TradePrivate(date);
    }

    trade->setValue(stod(strs[0]));
    trade->setClientSector(strs[1]);
    trade->setNextPaymentDate(strs[2]);
    entryList.push_back(trade);
  }

  // Print result
  for (int i = 0; i < entryList.size(); ++i) {
    entryList[i]->classifier();
    delete entryList[i];
  }
}
