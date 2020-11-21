#include "display/curses_subwin.hpp"
#include <string>

void SubWindow::box(chtype vline, chtype hline){
    if( this->win ){
        // ::box(win.get(), ((vline != '\0') ? vline: ACS_VLINE) , ((hline != '\0') ? hline: ACS_HLINE)); // calling the one available in global scope
        ::box(win.get(), vline , hline); // calling the one available in global scope
    }
    this->boxed = true;

    this->updateDimen();
}

void SubWindow::hline(chtype ch){
    this->updateDimen();
    ++last_write_pos.n_row;

    ::mvwhline(
        this->win.get(),
        last_write_pos.n_row,
        last_write_pos.n_col,
        ch,
        dimensions.n_col - (boxed ? 2: 0)   /*if boxed then create the line with (width-2)*/
    );
}

void SubWindow::refresh(){
    ::wrefresh(this->win.get());
    this->updateDimen();
}

void SubWindow::refreshParent(){
    ::wrefresh(this->parent_win->win.get());
    this->updateDimen();
}

void SubWindow::disable(){
    if( !this->enabled )  return;

    if( this->win.use_count() == 1 ){ // @caution @ignore - Read about shared_ptr::unique and shared_ptr::use_count(), these are not really accurate in multi-threading cases, where another thread has a reference to this shared_ptr
        delwin(this->win.get());
    }
    this->win.reset();
    dimensions.n_col = 0;
    dimensions.n_row = 0;

    this->enabled = false;
}

void SubWindow::enable(){
    if( this->enabled )  return;

    if( !this->win ){
        if( !this->parent_win )
            this->win.reset(subwin(stdscr, height, width, y_start, x_start));
        else
            this->win.reset(subwin(this->parent_win->win.get(), height, width, y_start, x_start));
    }

    this->enabled = true;
    this->updateDimen();
}

// @note - It is expected that updateDimen() was called recently before accessing these getmax methods
int SubWindow::getmax_x(){
    return this->dimensions.n_row;
}
int SubWindow::getmax_y(){
    return this->dimensions.n_col;
}

void SubWindow::updateDimen(){
    // @note - May require mutex locks, to prevent multiple threads trying to wriet to the same thing
    getmaxyx(win.get(), dimensions.n_col, dimensions.n_row);
}

void SubWindow::addch(char ch){
    waddch(this->win.get(), ch);
}

void SubWindow::addstr(std::string_view str, position pos){
    if( pos != position::AT_CURS ){
        this->updateDimen();
        if( pos == position::LEFT )
            if( boxed ) this->moveCursor(last_write_pos.n_row, 1);
            else this->moveCursor(last_write_pos.n_row, 0);
        else if( pos == position::RIGHT )
            if( boxed ) this->moveCursor(last_write_pos.n_row, -1);
            else this->moveCursor(last_write_pos.n_row, dimensions.n_col);
        else if( pos == position::MIDDLE )
            this->moveCursor(last_write_pos.n_row, (dimensions.n_col - str.size()) / 2);
    }

    // if boxed, then see if, (str.size() - 2), 2 for the border (right and left)
    if( (str.size() - (boxed ? 2 : 0)) > this->dimensions.n_col ){
        // @todo - Case when string length is larger than available screen space
    }
    waddstr(this->win.get(), str.data());
}

/**
 * @note - If either of row or col is negative, then it will be understood as max-the_value, for eg. row = -1, will be row = getmaxx(this->win.get())-1
*/
void SubWindow::addstr(int row, int col, std::string_view str){
    if( row < 0 )
        row = getmaxy(this->win.get()) - row;

    if( col < 0 )
        col = getmaxx(this->win.get()) - col;

    this->moveCursor(row, col);
    return this->addstr(str);
}

// move to the next line (NL), and add str
void SubWindow::nladdstr(std::string_view str, position pos){
    this->newline();
    this->addstr(str, pos);
}

void SubWindow::printf(const char* format){
    this->addstr(format);
}

void SubWindow::moveCursor(int row, int col){
    wmove(this->win.get(), row, col);

    last_write_pos.n_row = row;
    last_write_pos.n_col = col;
}

void SubWindow::newline(){
    this->moveCursor(last_write_pos.n_row + 1, last_write_pos.n_col);
}
