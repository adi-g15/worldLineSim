#[repr(C)]
struct Ear {
    friendly_factor: f32
}

impl Ear() {
    pub fn new() -> Ear {
        Ear { friendly_factor: 1.0 }
    }

    /*
     * When person of opposite gender in front... increase this :D :D :D :D :D :D :D
     * */
    pub fn set_friendly(factor: f32) {
        friendly_factor = factor;
    }

    pub fn hear(heard_data: &[u8]) -> f32 {
        let mut is_the_person_good = 0;
        let good_val = friendly_factor * heard_data.size();  // the threshhold after which the person feels more friendly/good
        // NOTE: this good_val should be lower than 128 on average, ie. have a friendly nature for
        // average person

        for byte in heard_data.iter() {
            is_the_person_good += if byte > good_val { 1 } else { 0 };
        }

        is_the_person_good as f32 / heard_data.size() as f32
    }
}
