mod addons;
mod args;
mod core;
mod magic;
mod pack;
mod unpack;

use args::{parse_args, Args, Flags};
use pack::create_archive;
use simple_logger::SimpleLogger;
use std::io::{Error, ErrorKind, Result};
use unpack::unpack_archive;

const VERSION: &str = "arcpack-rs version 2.0";
const HELP: &str = r#"Arcpack-rs, continuation of Arcpack
usage: <Options> <Files>
Options:
    -o # specify output file
    --help # show this message
    --version # show version info
    -c # make archive
    -d # unpack archive
"#;

fn main() -> Result<()> {
    SimpleLogger::new().init().unwrap();
    let args = parse_args()?;
    info(&args);

    if args.contains(Flags::DeflateArchive) && args.has_outfile() {
        log::error!("Cannot have output specified when unpacking");
        return Err(Error::new(ErrorKind::NotSeekable, "error 943"));
    }

    if !args.contains(Flags::DeflateArchive) && !args.contains(Flags::CreateArchive) {
        log::error!("Mode must be specified!");
        return Err(Error::new(ErrorKind::NotFound, "error 948"));
    }

    if args.contains(Flags::DeflateArchive) {
        log::info!("Unpacking Archive");
        for i in args.files.iter() {
            unpack_archive(&i)?;
        }
    } else if args.contains(Flags::CreateArchive) {
        log::info!("Creating Archive");
        create_archive(&args.files, args.get_outfile(), args.has_outfile())?;
    }

    Ok(())
}

fn info(args: &Args) {
    let mut info: bool = false;
    if args.contains(Flags::Help) {
        info = true;
        println!("{}", HELP);
    }
    if args.contains(Flags::Version) {
        info = true;
        println!("{}", VERSION);
    }
    if info {
        std::process::exit(0);
    }
}
