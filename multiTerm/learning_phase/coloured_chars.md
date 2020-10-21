Will be Useful in Ludo The Game ->

> this currently prints due to noecho disabled

```cpp
    if( has_colors() ){
        start_color();

        /*
            COLOR ASSIGNMENT. Not necessary to do like here only
        */
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_GREEN, COLOR_WHITE);
        init_pair(3, COLOR_YELLOW, COLOR_RED);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_CYAN, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_WHITE, COLOR_BLACK);
    }

    int num = 0;

    while (true)
    {
        int c = getch();
        attrset(COLOR_PAIR( num%8 ));
        num++;

    }
```