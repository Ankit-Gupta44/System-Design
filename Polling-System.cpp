#include <bits/stdc++.h>


using namespace std;
#define ff first
#define ss second
// cp - 1 q, mcq;
//

class User{
    
    public:
    string name;
    int id;
    
    User(string name ){
        
        this->name = name;
        // this->id = id;
        
    }
    
    
};


class Polls{
    public:
    int id;
    string question;
    map<string,int> options;
    string time_stamp;
    map<int,bool> user_voted;
    bool polling_time_over;
    
    Polls(int id, string &ques, vector<string>&opt){
        
        this->id = id;
        question = ques;
        
        for(auto &e : opt){
            
            options[e] = 0; // intiall
            
        }
        
        // time_stamp = time_created_at;  to be
        polling_time_over = false;
        
    }
    void updateData( string &ques, vector<string>&opt ){
        
        // this->id = id;
        question = ques;
        
        for(auto &e : opt){
            
            options[e] = 0; // intiall
            
        }
        
        // time_stamp = time_created_at;
        
    }
    
    bool vote( int &userId, string &opt ){
        
        if( options.find(opt) == options.end() ){
            
            cout<<" Option does not exist "<<endl;
            return false;
            
        }
        
        if( polling_time_over ){
            
            cout<<"poll ended"<<endl;
            return false;
            
        }
        
        if( user_voted.find(userId) != user_voted.end() ){
            
            cout<<" user alredy voted "<<endl;
            return false;
            
        }
        
        options[opt]++;
        user_voted[userId] = true;
        return 1;
    }
    
    void view_result(){
        
        cout<<question<<endl;
        
        for(auto &e : options){
            
            cout<<" option: "<<e.ff<<"  : "<<e.ss<<endl;
            
        }
        
    }
    
    void endPoll(){
        
        if( polling_time_over ){
            
            cout<<"polling time already over"<<endl;
            
        }
        
        polling_time_over = true;
        
    }
    
    
};


class Polling_system{
    
    public:
    
    int pll_id;
    int user_id;
    map<int,Polls*> mp;
    map<int,User*> users;
    
    Polling_system(int a,int b) : pll_id(a), user_id(b){}
    
    int createPoll(  string &ques, vector<string>&opt ){ // return pollid
        
        Polls* poll = new Polls(pll_id, ques, opt);
        
        mp[ pll_id ] = poll;
        pll_id++;
        
        
        return pll_id - 1;
    }
    
    bool updatePoll( int id, string &ques, vector<string>&opt ){
        
        if( mp.find(id) == mp.end() ){
            
            cout<<"NO poll exist for given poll id"<<endl;
            return false;
        }
        
        mp[id]->updateData(ques,opt);
        
        
       return true; 
        
    }
    
    bool Vote( int userId, int Pollid, string opt ){
        
        if( mp.find(Pollid) == mp.end() || users.find(userId) == users.end() ){
            
            cout<<" invalid User or Poll_id "<<endl;
            return false;
        }
        
        bool voted = mp[Pollid]->vote( userId , opt );
        
        if( !voted ){
            
            cout<<" something went wring "<<endl;
            return false;
            
        }
        else {
            
            cout<<" voted succesfully "<<endl;
            return true;
            
        }
        
    }
    
    void View_result( int poll_id ){
        
        if( mp.find(poll_id) == mp.end() ){
            
            cout<<"NO poll exist for given poll id"<<endl;
            return ;
        }
        
        Polls* poll = mp[poll_id];
        
        poll->view_result();
        
    }
    
    void deletePoll(int poll_id){
        
        Polls* poll = mp[poll_id];
        
        poll->endPoll();
        
    }
    
    int addUser( User* user ){ // return id
        
        user->id = user_id;
        users[user_id] = user;
        user_id++;
        
        return user_id - 1;
        
    }
    
    
};


int main() {
    
    Polling_system* obj = new Polling_system(0,0);
    
    User* user1 = new User("ankit");
    User* user2 = new User("harsh");
    
    int user1_id = obj->addUser(user1); 
    // user1->id = user1_id;
       
    int user2_id = obj->addUser(user2);
    // user2->id = user2_id;
    
    string ques = "what is ur fav color";
    vector<string> opt;
    
    opt.push_back("red");
    opt.push_back("green");
    opt.push_back("black");
    opt.push_back("blue");
    
    int pollid = obj->createPoll( ques, opt);
    
    
    obj->Vote(user1_id, pollid, opt[0] );
    obj->Vote(2, pollid, "grey" );
    obj->Vote(user2_id, pollid, opt[1] );
    
    obj->View_result(pollid);
    
    //update
    opt.push_back("pink");
    obj->updatePoll( pollid, ques, opt);
    obj->View_result(pollid);
    
        
    return 0;
}


