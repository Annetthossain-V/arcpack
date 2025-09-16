use crate::addons::human_data;
use crate::core::{write_dir_metadata, write_file_metadata, write_file_u16, write_header};
use crate::magic::{DIR_DATA_END, FILE_DATA_END};
use lazy_static::lazy_static;
use std::fs::{File, read_dir};
use std::io::{Error, ErrorKind, Read, Result, Write};
use std::path::Path;
use std::sync::Mutex;

lazy_static! {
    static ref PATH: Mutex<Vec<String>> = Mutex::new(Vec::new());
}

pub fn create_archive(files: &Vec<String>, outfile: Option<&str>, is_outfile: bool) -> Result<()> {
    if files.len() > 1 && is_outfile == false {
        log::error!("When archiving multiple files, out file must be specified");
        return Err(Error::new(ErrorKind::NotSeekable, "error 143"));
    }

    let out_file: String;
    if is_outfile {
        out_file = outfile.unwrap().to_string();
    } else {
        log::error!("dynamic file name support not added yet!");
        todo!();
    }
    log::info!("creating archive `{}`", out_file);
    let mut file: File = File::create_new(&out_file)?;
    write_header(&mut file);

    for file_name in files.iter() {
        if file_name.to_string() == out_file {
            continue; // cheap solution
        }
        let mut file_name: String = file_name.to_owned();

        if file_name.chars().nth(file_name.len() - 1).unwrap() != '/' {
            file_name.push('/');
        }

        PATH.lock().unwrap().push(file_name);
        let mut path = String::new();

        for i in PATH.lock().unwrap().iter() {
            path.push_str(&i);
        }

        type_handler(Path::new(&path), &mut file)?;
        PATH.lock().unwrap().pop();
    }

    Ok(())
}

fn type_handler(file_name: &Path, file: &mut File) -> Result<()> {
    log::info!("Reading `{}`", file_name.display());

    if file_name.is_file() {
        file_handler(file_name, file);
    } else if file_name.is_dir() {
        for entry in read_dir(file_name)? {
            let entry = entry?;
            let path = entry.path();

            if path.is_dir() {
                dir_handler(&path, file);
            } else if path.is_file() {
                file_handler(&path, file);
            } else {
                log::error!(
                    "expected directory or file, found `{}`. item unsupported",
                    path.display()
                );
                return Err(Error::new(ErrorKind::InvalidInput, "error 555"));
            }
        }
    } else {
        log::error!(
            "expected directory or file, found `{}`. item unsupported",
            file_name.display()
        );
        return Err(Error::new(ErrorKind::InvalidInput, "error 553"));
    }

    Ok(())
}

fn file_handler(file_name: &Path, file: &mut File) {
    let mut path: String = String::new();
    for i in PATH.lock().unwrap().iter() {
        path.push_str(&i);
    }
    path.push_str(file_name.file_name().unwrap().to_str().unwrap());
    log::info!("Writing File `{}`", &path);

    let mut copy_file: File = File::open(&path).unwrap();
    let cp_file_metadata = copy_file.metadata().unwrap();

    write_file_metadata(
        file,
        file_name.to_str().unwrap(),
        &path,
        cp_file_metadata.len() as u32,
    )
    .unwrap();

    log::info!("copying file data!");

    let mut cpy_size: u32 = cp_file_metadata.len() as u32;
    let mut data_chunk: Vec<u8> = vec![0u8; 52428800];
    while cpy_size > 0 {
        if cpy_size < 52428800 {
            data_chunk.truncate(cpy_size as usize);
        }

        let rba = copy_file.read(&mut data_chunk).unwrap();
        file.write(&data_chunk).unwrap();
        cpy_size -= rba as u32;

        let hurb = human_data(cpy_size as u64);
        print!("{hurb} left!\r");
        std::io::stdout().flush().unwrap();
    }

    write_file_u16(file, FILE_DATA_END);
}

fn dir_handler(file_name: &Path, file: &mut File) {
    let mut name = file_name.file_name().unwrap().to_str().unwrap().to_owned();
    if name.chars().nth(name.len() - 1).unwrap() != '/' {
        name.push('/');
    }

    PATH.lock().unwrap().push(name);
    let mut path: String = String::new();
    for i in PATH.lock().unwrap().iter() {
        path.push_str(&i);
    }
    log::info!("Writing directory `{}`", &path);

    write_dir_metadata(
        file,
        file_name.file_name().unwrap().to_str().unwrap(),
        &path,
    )
    .unwrap();
    type_handler(file_name, file).unwrap();
    write_file_u16(file, DIR_DATA_END);

    PATH.lock().unwrap().pop();
}
