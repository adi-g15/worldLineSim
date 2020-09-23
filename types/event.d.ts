import { Direction } from "./direction";
import { Point } from "./point";

// export {}   //to have module scope

export interface Event{
    moves_by_snakes: [Direction, Direction, Direction, Direction];  //moves by each snake
    curr_food_pos: Point;
};

export interface indEvent{  //for a particular snake
    move_by_this_snakes: Direction;  //move by this snake
    curr_food_pos: Point;
};
