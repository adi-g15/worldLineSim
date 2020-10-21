"use strict";
exports.__esModule = true;
exports.random = exports.last = void 0;
function last(collection) {
    return collection[collection.length - 1];
}
exports.last = last;
function random(start, end) {
    if (!end) {
        end = start;
        start = 0;
    }
    return Math.floor(Math.random() * (end + 1 - start) + start);
}
exports.random = random;
