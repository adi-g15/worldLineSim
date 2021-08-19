use rand::{thread_rng, RngCore};

#[repr(C)]
struct Mouth {}

impl Mouth {
    /*
     * Note: Currently just spouts random data
     * */
    pub fn speak() -> Vec<u8> {
        let mut data = [u8; random_number(5,50) ];  // each time we don't speak same amount of things... hai na :D
        
        thread_rng().fill_bytes(&mut data);

        data.to_vec()
    }
}
