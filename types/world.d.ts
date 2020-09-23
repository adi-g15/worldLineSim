import { Event } from "./event";
import { Snake } from "./snake";
import { Point } from "./point";

class World{
    snakes: [Snake, Snake, Snake, Snake];
    _event_log: Event[];
    _curr_food_pos: Point;
    /*const*/ _max_X = 15;
    /*const*/ _max_Y = 15;

    constructor(oldWorld?: World, params?: object){
        this.snakes = [new Snake, new Snake, new Snake, new Snake];
        this._event_log = [];
        this._curr_food_pos = {
            x: Math.random()*this._max_X,
            y: Math.random()*this._max_Y
        };

        if( oldWorld ){
            if( !params ) throw new Error("Params need to be passed along with the old World");
            
            // construct a new world, using oldWorld data, and params passed, such as time point
        }
    }

};

function createNewWorld(oldWorld?:World, params?: object): World {
}

function shortestDist(world:World, p1: Point, p2: Point) {

    //for now let the world be free of any interference, so not using the world now
    /*
    
    

    use queue... FIFO
    call(node)
    for node:
        q.push neighbours
        call(q.top())
    
    */

}

export {
    World,
    createNewWorld
}