#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <chrono>
#include <algorithm>
#include <unordered_map>
using namespace std::chrono;
using namespace std;



// This code is written by Aman kumar
// Email-amansingh.nitrkl@gmail.com
// date and time of creation = 10th april 2023, 12:16am 


struct Trip {
    int id;
    string start_date;
    int start_time;
    string end_date;
    int end_time;
    double start_lat;
    double start_lng;
    double end_lat;
    double end_lng;
};

struct Eucli{
    double edistance;
    double slat;
    double slng;
    double elat;
    double elng;
};

// converting the time string into int minutes
int timeStringToIntMinutes(string timeStr) {
    int hours = stoi(timeStr.substr(0, 2));
    int minutes = stoi(timeStr.substr(3, 2));
    return hours * 60 + minutes;
}


// printing the data
void printing(vector<Trip> trips){
    for(auto i:trips){
        cout<<i.id<<" "<<i.start_date<<" "<<i.start_time<<" "<<i.end_date<<" "<<i.end_time<<" ";
        cout<<i.start_lat<<" "<<i.start_lng<<" "<<i.end_lat<<" "<<i.end_lng<<endl;
    }
}

// function to remove the the data whose duration is zero
void removeTheZeroDuration(vector<Trip> &trips, vector<Trip> &deletedTripsWithZero){

    for (int i = 0; i < trips.size(); i++) {
        if (trips[i].start_time == trips[i].end_time) {
            deletedTripsWithZero.push_back(trips[i]);
            trips.erase(trips.begin() + i);
            i--;
        }
    }
}

// finding the maximum and minimum duration of trip
void FindingMaxMin(int &maxi, int &mini, vector<Trip> &trips){

    for(int i=0; i<trips.size(); i++){
        if ((trips[i].end_time - trips[i].start_time) > maxi){
           maxi = (trips[i].end_time - trips[i].start_time);
        }

        if((trips[i].end_time - trips[i].start_time) < mini){
            mini = (trips[i].end_time - trips[i].start_time);
        }
    }

}

// finding the maximum and minimum count
void FindingTheCount(int maxi, int mini, vector<Trip> &trips, int &max_count, int &min_count){
    for(int i=0; i<trips.size(); i++){
        if((trips[i].end_time - trips[i].start_time) == maxi){
            max_count++;
        }

        if((trips[i].end_time - trips[i].start_time) == mini){
            min_count++;
        }
    }
}

// finding the circular count in the trip
void FindCircular(vector<Trip> &trips, int &circularcount){
    for (int i = 0; i < trips.size(); i++)
    {
        if((trips[i].start_lat == trips[i].end_lat) && (trips[i].start_lng == trips[i].end_lng)){
            circularcount++;
        }
    }
}






// this is for part 2 question 1

void filterningdata(vector<Trip> &filterdata, vector<Trip> &trips){
    for (int i = 0; i < trips.size(); i++){
        if(trips[i].start_time >= 360 and trips[i].start_time <= 1080){
            filterdata.push_back(trips[i]);
        }
    }
}

// finding feasible count
void feasible(int &feasible_count, vector<Trip> &filterdata){

    for (size_t i = 0; i < filterdata.size() - 1; i++) {

        for (size_t j = i + 1; j < filterdata.size(); j++) {
            if ((filterdata[i].end_lat == filterdata[j].start_lat) && (filterdata[i].end_lng == filterdata[j].start_lng) && 
            (filterdata[i].end_time <= filterdata[j].start_time)) {
                feasible_count++;
            }
        }
    }
    
} 

//optimized method code for counting the number of feasible pair
void checkfeasible(vector<Trip> &filterdata, int &feasible_count){

    unordered_map<string, vector<Trip> > tripsByEndLocation;

    for (int i=0; i<filterdata.size(); i++) {
        string temp=to_string(filterdata[i].end_lat) + "=" + to_string(filterdata[i].end_lng);
        tripsByEndLocation[temp].push_back(filterdata[i]);
    }

    // Count feasible pairs of trips
    for (const auto& tripA : filterdata) {
        string temp2=to_string(tripA.start_lat) + "=" + to_string(tripA.start_lng);

        auto it = tripsByEndLocation.find(temp2);
        if (it != tripsByEndLocation.end()) {
            const auto& tripsB = it->second;
            for (const auto& tripB : tripsB) {
                if (tripB.start_time >= tripA.end_time) {
                    feasible_count++;
                }
            }
        }
    }
}





// this is the start of 3 part of the 1 question

// putting the data in lastpartdata for first 100 id
void lastpart(vector<Trip> &lastpartdata, vector<Trip> &trips){
    int i=0;
    while(trips[i].id<=100){
        lastpartdata.push_back(trips[i]);
        i++;
    }
    return;
}

// count the number of unique depots in the data
void countUniqueDepots(vector<Trip> &lastpartdata, int &uniquedepots, unordered_map<string, int> &checkdata){

    for(int i=0; i<lastpartdata.size(); i++){
        string temp=to_string(lastpartdata[i].start_lat) + "=" + to_string(lastpartdata[i].start_lng);
        checkdata[temp]++; 
    }
    uniquedepots=checkdata.size();
}


// finding the distance between depots
void findingTheDistance(unordered_map<string, int> &checkdata, vector<Eucli> &euclideandistance){

    unordered_map<string, int>::iterator it;
    for (it = checkdata.begin(); it != checkdata.end(); ++it) {

        string temp1=it->first;

        // Find the position of the equal sign (=) in the string
        size_t pos = temp1.find("=");

        // Extract the substrings before and after the equal sign
        string x_str = temp1.substr(0, pos);
        string y_str = temp1.substr(pos + 1);

        // Convert the substrings to double values
        double x1 = stod(x_str);
        double y1 = stod(y_str);

        unordered_map<string, int>::iterator its;
        for (its = it; its != checkdata.end(); ++its) {
            string temp2=its->first;

            // Find the position of the equal sign (=) in the string
            size_t pos2 = temp2.find("=");

            // Extract the substrings before and after the equal sign
            string x_str2 = temp2.substr(0, pos2);
            string y_str2 = temp2.substr(pos2 + 1);

            // Convert the substrings to double values
            double x2 = stod(x_str2);
            double y2 = stod(y_str2);
            
            double dis=sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

            Eucli eucli;

            eucli.edistance=dis;
            eucli.slat=x1;
            eucli.slng=y1;
            eucli.elat=x2;
            eucli.elng=y2;
            

            if(eucli.edistance!=0){
                euclideandistance.push_back(eucli);
            }
        }     
    }
}

// this function print the distance and the position coordinate between them
void printthevector(vector<Eucli> &euclideandistance){
    for (int i = 0; i < euclideandistance.size(); i++){
        cout<<"The distance between "<<euclideandistance[i].slat<<", "<<euclideandistance[i].slng<<" and "
        <<euclideandistance[i].elat<<", "<<euclideandistance[i].elng<<" is "<<euclideandistance[i].edistance<<endl;
    }
    cout<<endl;
}

// find the maximum and minimum value of distance between the depots
void findTheMaxMindistance(vector<Eucli> &euclideandistance, double &maxDis, double &minDis){
    for(int i=0; i<euclideandistance.size(); i++){
        if(euclideandistance[i].edistance >  maxDis)
            maxDis = euclideandistance[i].edistance;
        
        if(euclideandistance[i].edistance  < minDis)
            minDis = euclideandistance[i].edistance;
    }
    return ;
}




int main() {
    vector<Trip> trips;
    string file_path = "bike_data_new.csv";

    ifstream input_file(file_path);
    if (!input_file) {
        cerr << "Unable to open file " << file_path << endl;
        return 1;
    }

    string line;
    getline(input_file, line);
    while (getline(input_file, line)) {
        stringstream ss(line);
        string token;
        Trip trip;

        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid trip ID" << endl;
            continue;
        }
        trip.id = stoi(token);




        // start date
        if (!getline(ss, token, ' ') || token.empty()) {
            cerr << "Invalid start date" << endl;
            continue;
        }
        trip.start_date = token;

        // start time
        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid start time" << endl;
            continue;
        }
        trip.start_time = timeStringToIntMinutes(token);

        // end date
        if (!getline(ss, token, ' ') || token.empty()) {
            cerr << "Invalid end time" << endl;
            continue;
        }
        trip.end_date = token;

        // end time
        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid end time" << endl;
            continue;
        }
        trip.end_time = timeStringToIntMinutes(token);




        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid start latitude" << endl;
            continue;
        }
        trip.start_lat = stod(token);

        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid start longitude" << endl;
            continue;
        }
        trip.start_lng = stod(token);

        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid end latitude" << endl;
            continue;
        }
        trip.end_lat = stod(token);

        if (!getline(ss, token, ',') || token.empty()) {
            cerr << "Invalid end longitude" << endl;
            continue;
        }
        trip.end_lng = stod(token);

        trips.push_back(trip);
    }
    input_file.close();

    // Perform analysis on trips here

    // timer start
    auto start_time = high_resolution_clock::now();
    // Call the function




    vector<Trip> deletedTripsWithZero;

    removeTheZeroDuration(trips, deletedTripsWithZero);
    // printing(deletedTripsWithZero);
    // printing(trips);

    int maxi=INT_MIN;
    int mini=INT_MAX;
    FindingMaxMin(maxi, mini, trips);
    cout<<"Maximum duration of the trip -> "<<maxi<<" minutes"<<endl;
    cout<<"Minimum duration of the trip -> "<<mini<<" minutes"<<endl;
    cout<<endl;

    int max_count=0;
    int min_count=0;
    FindingTheCount(maxi, mini, trips, max_count, min_count);
    cout<<"Total number of trip corresponding to the maximum duration -> "<<max_count<<endl;
    cout<<"Total number of trip corresponding to the minimum duration -> "<<min_count<<endl;
    cout<<endl;

    int circularcount=0;
    float totalctrips = trips.size();
    FindCircular(trips, circularcount);
    cout<<"Percentage of total circular trips -> "<<(circularcount*100)/totalctrips<<"%"<<endl;



    // Stop the timer
    auto end_time = high_resolution_clock::now();
    // Calculate the duration
    auto duration = duration_cast<microseconds>(end_time - start_time);
    // Print the duration in milliseconds
    std::cout << "Function runtime: " << duration.count() / 1000.0 << " ms" << std::endl;


    // end of the part 1 question 1



    // below code is for 2nd part question 1


    // timer start
    auto start_time1 = high_resolution_clock::now();
    // Call the function

    vector<Trip> filterdata;
    filterningdata(filterdata, trips);


    int feasible_count=0;

    // brute force method
    // feasible(feasible_count, filterdata);
    // cout<<"Total number of feasible pair is -> "<<feasible_count<<endl;

    // optimize method
    checkfeasible(filterdata, feasible_count);
    cout<<"Total number of feasible pair is -> "<<feasible_count<<endl;



    // Stop the timer
    auto end_time1 = high_resolution_clock::now();
    // Calculate the duration
    auto duration1 = duration_cast<microseconds>(end_time1 - start_time1);
    // Print the duration in milliseconds
    std::cout << "Function runtime -> " << duration1.count() / 1000.0 << " ms" << std::endl;

    // end of the question 1 part 2




    // below code is for 3rd question

    // timer start
    auto start_time2 = high_resolution_clock::now();
    // Call the function


    vector<Trip> lastpartdata;
    unordered_map<string, int> checkdata;
    lastpart(lastpartdata, trips);
    // printing(lastpartdata);

    int uniquedepots=0;
    countUniqueDepots(lastpartdata, uniquedepots, checkdata);
    cout<<"Total number of unique depots -> "<<uniquedepots<<endl;

    vector<Eucli> euclideandistance;
    findingTheDistance(checkdata, euclideandistance);
    
    // this is for printing the distance and the latitude and longititude of the deptos
    // printthevector(euclideandistance);

    double maxDis=INT_MIN;
    double minDis=INT_MAX;
    findTheMaxMindistance(euclideandistance, maxDis, minDis);
    cout<<"Maximum Distace -> "<<maxDis<<endl;
    cout<<"Minimum Distace -> "<<minDis<<endl;



    // Stop the timer
    auto end_time2 = high_resolution_clock::now();
    // Calculate the duration
    auto duration2 = duration_cast<microseconds>(end_time2 - start_time2);
    // Print the duration in milliseconds
    cout << "Function runtime -> " << duration2.count() / 1000.0 << " ms" << std::endl;




    return 0;
}

