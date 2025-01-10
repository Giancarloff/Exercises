use std::env;
use std::fs::File;
use std::path::Path;
use std::io::prelude::*;

fn main() {
    let args: Vec<String> = env::args().skip(1).collect();

    for arg in args {
        let path = Path::new(arg.as_str());
        let display = path.display();
        
        let mut file= match File::open(&path) {
            Err(why) => {
                println!("Failed to open file {}: {}.", display, why);
                continue;
            },
            Ok(file) => file,
        };

        let mut source = String::new();
        let size = match file.read_to_string(&mut source) {
            Err(why) => {
                println!("Failed to read file {} as string: {}.", display, why);
                continue;
            },
            Ok(size) => size,
        };
        
        #[cfg(debug_assertions)] 
        {
            if size > 0 {
                println!("Read {} bytes from: {} ", size, display);
                println!("{}", source);
            }
            println!("Running lexical analysis on {}.", display);
        }

    }

}