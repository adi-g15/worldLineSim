// mod human

#[repr(C)]
struct Brain {
    /*Internal State*/
    pub fn get_next_move() -> Option<Direction> {
        /*
         * Things to consider:
         * 1. Meeting people ->
         *      lower priority as neared to 300 friends
         *      gender matters  a bit
         *      share 'some' of past experience
         *      uses the 'Eye' to see if anyone near, talk with 'Mouth', the other party listens
         *      with 'Ear', most probably you will be 'friends', the more talk the higher chance
         * 2. Observation (the original code logic)
         *      random direction
         * 3. Ambition/Goal ->
         *      'Knowledge' (go to library), 'Mathematics' (reach mathematicians), 'Astronomy',
         *      'Mingle with people', 'Anime' (stays at current place, watches TV), 'Earn money',
         *      'Medidate' (attained universe's knowledge, no more moves, shares this knowledge
         *      with people who come)
         * 4. Prev Goal (ie. prev direction)
         **/

        unimplemented!();
    }
}

