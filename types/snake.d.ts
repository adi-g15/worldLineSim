import { indEvent } from "./event";
import { Point } from "./point";

export class Snake{
    size: number;
    coords: Point[];
    _log: indEvent[];

    constructor(startCoord?: Point){
        this.size = 3;
        this.coords = [startCoord];
        this._log = [];
    }
}