#[repr(C)]
struct Eye {
    current_facing_dir: Direction
}

impl Eye {
    pub fn new(facing_dir: Direction) -> {
        Eye {
            current_facing_dir = facing_dir
        }
    }

    pub fn turn(&mut self, degrees_to_right: f32) {
        self.current_facing_dir = /**/;

        unimplemented!();
    }

    pub fn get_near_objects() -> Vec<(Coord,EntityType)> {
        // Don't implement now, not needed

        unimplemented!();
    }

    pub fn find_near_humans() -> Vec<Coord> {
        /*
         * See 120 degree in front (ie. till 60 degree on both side)
         **/

        unimplemented!();
    }
}
