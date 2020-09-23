let canvas: HTMLCanvasElement = document.getElementById('plane') as HTMLCanvasElement;

// import { last, random } from "util";
// import {  } from "lodash";

function last(collection: any[]) {
    return collection[collection.length-1]
}

function random(start:number, end?: number): number {
    if( !end ){
        end = start;
        start = 0;
    }
    return Math.floor( Math.random()*(end+1-start) + start );
}


// NOTE - Coordinate System used throughout is x-axis form negative to positive in left to right direction
            // and y-axis from 0 to +infinity from top to bottom
// NOTE - 0 based indexing is to be used throughout

// Setting dimensions
const _USIZE = 20;   //the unit size
const HEIGHT = 15;
const WIDTH = 20;
canvas.height = HEIGHT*_USIZE;
canvas.width = WIDTH*_USIZE;

type Point = [number, number];
function randPoint(): Point {
    return [ random(WIDTH-1), random(HEIGHT-1) ]
}
function randColour(): string {
    let colour = (Math.random() * 0xfffff * 1000000).toString(16);

    return "#" + colour.slice(0,6);
}

function isValid(coord_x: number, coord_y: number) {
    return !( coord_x < 0 || coord_x >= HEIGHT || coord_y < 0 || coord_y >= WIDTH )
}

function randNeighbor(point:Point): Point {
/*  ----------
    | +| 0| +|
    ----------
    |1 |  |2 |
    ----------
    | +| 3| +|
    ----------  */

        // The coordinate may confuse a bit, but whole program only follows the canvas coordinates, stated in this file starting
    let index = random(3);
    let newPoint: Point;
    switch (index) {
        case 0: newPoint = [point[0], point[1] -1];   break;
        case 1: newPoint = [point[0] -1, point[1]];   break;
        case 2: newPoint = [point[0] +1, point[1]];   break;

        default:    newPoint = [point[0], point[1] +1];
    }
    if ( !isValid(...newPoint) || ! isSafe(newPoint) )
        return randNeighbor(point);

    return newPoint;
}

function isSafe(point: Point) {
    if( !isValid(...point) )    throw new Error("Invalid Point passed");

    for (let j = 0; j < 3; j++) {
        for (let i = 0; i < 3; i++) {
            if( (i===2 && j===2) || !isValid(point[0]-1+i, point[1]-1+j) ) continue;

            if( boardFlag[ point[0]-1+i ][ point[1]-1+j ] )
                return false;
        }
    }

    return true;
}

let boardFlag: boolean[][] = new Array< Array<boolean> >(15);
for (let i = 0; i < HEIGHT; i++) {
    boardFlag[i] = new Array<boolean>(WIDTH);
    boardFlag[i].fill(false);
}

// let ctx = canvas.getContext('webgl2');
let ctx = canvas.getContext('2d');  //disable transparency
// let ctx = canvas.getContext('2d', {alpha: false});  //disable transparency

if( !ctx )
    alert("Couldn't get the context to draw on canvas, please reload");

else{

type Colour = string;   //basically a typedef for string

ctx = ctx as CanvasRenderingContext2D;  //Since it's inside else block, so it's safe to do so, and prevents further improbable errors
class Snake{
    _snake_Id: number;

    head: Point;
    colour: Colour;
    body: Point[];   //includes head too
    score: number;

    constructor(head?: Point, colour?: Colour){
        if( !head ) head = randPoint();
        if( !colour ) colour = randColour();

        
        this.head = head;
        this.colour = colour;
        this.body = [this.head];
        this.score = 0;
    }
    
    /**
     * growBody
     */
    public growBody() {
        
    }
};

let snakes : Snake[] = [];
let numSnakes = 4;

for (let i = 0; i < numSnakes; i++) {
    snakes.push(new Snake());
}

snakes.forEach(snake => {
    let point = randPoint();
    while( !isValid(...point) ) point = randPoint();
    while ( ! isSafe(point) ){
        point = randPoint();
    }
    snake.head = point;
    snake.body.push(point);

    for(let i in [1,2,3]){
        point = randNeighbor( last(snake.body) as Point );    //snake.body will surely be >= 1, since it does have a head
        while( ! isSafe(point) ){
            point = randNeighbor( last(snake.body) as Point );
        }
        snake.body.push( point );
    }

    console.log(snake.body);
});
snakes[0].colour = randColour();
snakes[1].colour = randColour();
snakes[2].colour = randColour();
snakes[3].colour = randColour();

let start: Point = [10, 10];
let dimen: Point | Point = [100,100]
ctx.fillStyle = 'white';
ctx.fillRect(...start, ...dimen);

for (let h = 0; h < HEIGHT; h++) {
    ctx.strokeStyle = 'red';
    ctx.moveTo(0, h*_USIZE);
    ctx.lineTo(WIDTH*_USIZE, h*_USIZE);
    console.log("Drawing from ",0, h*_USIZE, " to ", WIDTH*_USIZE, h*_USIZE)
}
ctx.stroke();

dimen = [_USIZE, _USIZE]
snakes.forEach(snake => {
    if(ctx) ctx.fillRect(snake.head[0]*_USIZE, snake.head[1]*_USIZE, ...dimen);
    snake.body.slice(1).forEach(block => {
        start = [block[0]*_USIZE, block[1]*_USIZE];
        if(ctx) ctx.fillRect(...start, ...dimen);
    });
});


}