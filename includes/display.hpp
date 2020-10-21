#include <memory>

// #include "verse.hpp" // 
#include "forward_decl.hpp"
#include "multiTerm/single_term.hpp"

/* This class will just `hold` everything, with all handling of `its members` being controlled by the parent verse object */
class Display : public single_term{
    Verse* parent_verse;
    ~Display(); //private destructor, only the Verse using it can destroy the Display

    public:
    void init() override;

    Display() = delete;
    Display(Verse&);

    friend class Verse;

};

void Display::init(){
    refreshAll();

    this->printMsg(0, this->title, 0, this->_win_Dimensions[0].second / 2);
    this->printMsg(0, this->title, 1, this->_win_Dimensions[0].second / 2);
}

Display::Display(Verse& parent) :
    single_term("WorldLine Simulator v0.1", "Created by Aditya Gupta and Steins; Gate"),
    parent_verse(&parent){}

Display::~Display(){

}
