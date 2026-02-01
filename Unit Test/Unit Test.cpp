#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

/* =====================================================
   ENUM: Types of vehicles
   ===================================================== */
enum VehicleType
{
    GT3 = 1,
    Formula,
    Rally
};

/* =====================================================
   STRUCT: Stores one racing session
   ===================================================== */
struct Session
{
    string driverName;
    string trackName;
    VehicleType vehicle;
    double lapTimes[3];
};

/* =====================================================
   CLASS: Manages all sessions and calculations
   ===================================================== */
class MotorsportManager
{
private:
    static const int MAX_SESSIONS = 5;
    Session sessions[MAX_SESSIONS];
    int sessionCount;

public:
    // Constructor
    MotorsportManager()
    {
        sessionCount = 0;
    }

    // Adds a session to the array
    bool addSession(const Session& s)
    {
        if (sessionCount >= MAX_SESSIONS)
            return false;

        sessions[sessionCount] = s;
        sessionCount++;
        return true;
    }

    // Returns how many sessions are stored
    int getSessionCount() const
    {
        return sessionCount;
    }

    // Calculates the average lap for one session
    double calculateAverageLap(const Session& s) const
    {
        double total = 0.0;

        for (int i = 0; i < 3; i++)
            total += s.lapTimes[i];

        return total / 3.0;
    }

    // Calculates average lap time across all sessions
    double calculateOverallAverage() const
    {
        if (sessionCount == 0)
            return 0.0;

        double total = 0.0;
        int lapCount = 0;

        for (int i = 0; i < sessionCount; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                total += sessions[i].lapTimes[j];
                lapCount++;
            }
        }

        return total / lapCount;
    }

    // Returns a base lap time based on vehicle type
    double getBaseLapTime(VehicleType vehicle) const
    {
        if (vehicle == GT3)
            return 95.0;
        else if (vehicle == Formula)
            return 70.0;
        else
            return 120.0;
    }
};

#ifndef _DEBUG
/* =====================================================
   MAIN PROGRAM (Normal Mode)
   ===================================================== */
int main()
{
    srand(static_cast<unsigned>(time(0)));

    MotorsportManager manager;
    Session session;

    cout << "========================================\n";
    cout << "     Welcome to Motorsports Simulator\n";
    cout << "========================================\n\n";

    // Get user input
    cout << "Enter driver name: ";
    getline(cin, session.driverName);

    cout << "Enter track name: ";
    getline(cin, session.trackName);

    int choice;
    cout << "\nChoose a vehicle:\n";
    cout << "1. GT3\n";
    cout << "2. Formula\n";
    cout << "3. Rally\n";
    cout << "Choice: ";
    cin >> choice;

    session.vehicle = static_cast<VehicleType>(choice);

    // Generate lap times
    double baseTime = manager.getBaseLapTime(session.vehicle);

    for (int i = 0; i < 3; i++)
    {
        double randomOffset = (rand() % 1000) / 100.0;
        session.lapTimes[i] = baseTime + randomOffset;
    }

    manager.addSession(session);

    // Display results
    cout << fixed << setprecision(2);
    cout << "\nLap Times:\n";

    for (int i = 0; i < 3; i++)
        cout << "Lap " << i + 1 << ": " << session.lapTimes[i] << " seconds\n";

    cout << "\nAverage Lap Time: "
        << manager.calculateAverageLap(session) << " seconds\n";

    cout << "Overall Average: "
        << manager.calculateOverallAverage() << " seconds\n";

    // Save report to file
    ofstream file("report.txt");

    file << left << setw(15) << "Driver"
        << setw(15) << "Track"
        << setw(10) << "Vehicle"
        << setw(12) << "Avg Lap\n";

    string vehicleName;
    if (session.vehicle == GT3)
        vehicleName = "GT3";
    else if (session.vehicle == Formula)
        vehicleName = "Formula";
    else
        vehicleName = "Rally";

    file << left << setw(15) << session.driverName
        << setw(15) << session.trackName
        << setw(10) << vehicleName
        << setw(12) << manager.calculateAverageLap(session) << endl;

    file.close();

    cout << "\nReport saved to report.txt\n";
    return 0;
}
#endif

/* =====================================================
   UNIT TESTS (doctest)
   ===================================================== */

   // A) Calculations
TEST_CASE("Average lap calculation works")
{
    MotorsportManager m;
    Session s{ "Test", "Track", GT3,{100, 98, 102} };
    CHECK(m.calculateAverageLap(s) == doctest::Approx(100.0));
}

TEST_CASE("Overall average with no sessions")
{
    MotorsportManager m;
    CHECK(m.calculateOverallAverage() == 0.0);
}

TEST_CASE("Overall average with one session")
{
    MotorsportManager m;
    Session s{ "A", "B", Formula,{70, 71, 69} };
    m.addSession(s);
    CHECK(m.calculateOverallAverage() == doctest::Approx(70.0));
}

TEST_CASE("Zero lap times handled safely")
{
    MotorsportManager m;
    Session s{ "Z", "Z", GT3,{0, 0, 0} };
    CHECK(m.calculateAverageLap(s) == 0.0);
}

// B) Enum decision logic
TEST_CASE("GT3 base time")
{
    MotorsportManager m;
    CHECK(m.getBaseLapTime(GT3) == 95.0);
}

TEST_CASE("Formula base time")
{
    MotorsportManager m;
    CHECK(m.getBaseLapTime(Formula) == 70.0);
}

TEST_CASE("Rally base time")
{
    MotorsportManager m;
    CHECK(m.getBaseLapTime(Rally) == 120.0);
}

// C) Struct / array processing
TEST_CASE("Session count starts at zero")
{
    MotorsportManager m;
    CHECK(m.getSessionCount() == 0);
}

TEST_CASE("Adding session increases count")
{
    MotorsportManager m;
    Session s{ "A","T",GT3,{90,90,90} };
    m.addSession(s);
    CHECK(m.getSessionCount() == 1);
}

TEST_CASE("Session limit enforced")
{
    MotorsportManager m;
    Session s{ "X","Y",GT3,{1,1,1} };

    for (int i = 0; i < 5; i++)
        CHECK(m.addSession(s));

    CHECK(m.addSession(s) == false);
}

// D) Class methods
TEST_CASE("addSession returns true")
{
    MotorsportManager m;
    Session s{ "OK","OK",Rally,{100,100,100} };
    CHECK(m.addSession(s));
}

TEST_CASE("getSessionCount reflects added session")
{
    MotorsportManager m;
    Session s{ "A","B",GT3,{95,95,95} };
    m.addSession(s);
    CHECK(m.getSessionCount() == 1);
}
