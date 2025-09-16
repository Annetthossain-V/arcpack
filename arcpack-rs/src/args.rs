use std::{
    env::args,
    io::{Error, ErrorKind, Result},
};

#[derive(PartialEq)]
pub enum Flags {
    OutFile(String),
    CreateArchive,
    DeflateArchive,
    Help,
    Version,
}

pub struct Args {
    pub files: Vec<String>,
    pub flags: Vec<Flags>,
    // pub output: bool,
}

pub fn parse_args() -> Result<Args> {
    let args_unformatted: Vec<String> = args().skip(1).collect();
    let mut flags: Args = Args {
        files: Vec::new(),
        flags: Vec::new(),
        // output: false,
    };
    flags.files.reserve(10);
    flags.flags.reserve(10);

    for mut i in 0..args_unformatted.len() {
        let arg: &str = &args_unformatted[i];
        if arg.chars().nth(0).unwrap() == '-' {
            match arg {
                "-o" => {
                    if (i + 1) >= args_unformatted.len() {
                        log::error!("Output file not found");
                        return Err(Error::new(ErrorKind::AddrNotAvailable, "error 552"));
                    }
                    i += 1;
                    flags
                        .flags
                        .push(Flags::OutFile(args_unformatted[i].clone()));
                }
                "-c" => {
                    if flags.contains(Flags::DeflateArchive) {
                        log::error!("Multiple mode given");
                        return Err(Error::new(ErrorKind::AlreadyExists, "error 423"));
                    }
                    flags.flags.push(Flags::CreateArchive);
                }
                "-d" => {
                    if flags.contains(Flags::CreateArchive) {
                        log::error!("Multiple mode given");
                        return Err(Error::new(ErrorKind::AlreadyExists, "error 423"));
                    }
                    flags.flags.push(Flags::DeflateArchive);
                }
                "--help" | "-h" => flags.flags.push(Flags::Help),
                "--version" | "-v" | "--ver" => flags.flags.push(Flags::Version),
                _ => {
                    log::error!("Unknown Argument `{}`", arg);
                    return Err(Error::new(ErrorKind::NotFound, "error 453"));
                }
            }
        } else {
            flags.files.push(args_unformatted[i].clone());
        }
    }

    Ok(flags)
}

impl Args {
    pub fn contains(&self, flag: Flags) -> bool {
        for i in self.flags.iter() {
            if i == &flag {
                return true;
            }
        }
        return false;
    }
    pub fn has_outfile(&self) -> bool {
        self.flags.iter().any(|f| matches!(f, Flags::OutFile(_)))
    }
    pub fn get_outfile(&self) -> Option<&str> {
        self.flags.iter().find_map(|f| {
            if let Flags::OutFile(s) = f {
                Some(s.as_str())
            } else {
                None
            }
        })
    }
}
