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

export {
    last,
    random
}