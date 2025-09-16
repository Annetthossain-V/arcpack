use crate::addons::human_data;
use crate::core::{check_file_header, read_dir_metadata, read_file_metadata, read_file_u16};
use crate::magic::*;
use std::fs::{metadata, File};
use std::io::{Error, ErrorKind, Read, Result, Seek, SeekFrom, Write};
use std::path::Path;

pub fn unpack_archive(archive_name: &str) -> Result<()> {
    log::info!("Unpacking `{}`", archive_name);

    let path = Path::new(archive_name);
    if !path.exists() {
        log::error!("File `{}` not found", archive_name);
        return Err(Error::new(ErrorKind::NotFound, "error 432"));
    }

    let file_info = metadata(path)?;
    if file_info.is_dir() || file_info.is_symlink() {
        log::error!("Not a file, item type unsupported! `{}`", archive_name);
        return Err(Error::new(ErrorKind::NotFound, "error 431"));
    }
    if !check_file_header(archive_name) {
        log::error!("invalid file header `{}`", archive_name);
        return Err(Error::new(ErrorKind::InvalidData, "error 430"));
    }

    let mut file = File::open(path)?;
    file.seek(SeekFrom::Start(9))?;
    type_handler(&mut file)?;
    Ok(())
}

fn type_handler(file: &mut File) -> Result<()> {
    log::info!("Reading Signatures");
    let sig: u16 = read_file_u16(file);

    match sig {
        END_OF_FILE => return Ok(()),
        FILE_BEGIN => file_handler(file).unwrap(),
        DIR_BEGIN => dir_handler(file).unwrap(),

        DIR_DATA_END => {
            file.seek(SeekFrom::Current(-2)).unwrap();
        }
        FILE_DATA_END => {
            file.seek(SeekFrom::Current(-2)).unwrap();
        }
        _ => {
            log::error!("Unknown file signature `{}`", sig);
            return Err(Error::new(ErrorKind::NotFound, "error 323"));
        }
    }

    Ok(())
}

fn dir_handler(file: &mut File) -> Result<()> {
    let dir_metadata = read_dir_metadata(file);
    log::info!("Unpacking Dir `{}`", dir_metadata.path);

    if read_file_u16(file) != DIR_DATA_BEGIN {
        log::error!("expected dir_data_begin signature! file may be corrupted");
        return Err(Error::new(ErrorKind::InvalidData, "error 331"));
    }

    std::fs::create_dir_all(dir_metadata.path)?;

    loop {
        type_handler(file).unwrap();
        let dir_end_magic = read_file_u16(file);
        if dir_end_magic == FILE_BEGIN || dir_end_magic == DIR_BEGIN {
            file.seek(SeekFrom::Current(-2))?;
            continue;
        }

        if dir_end_magic != DIR_DATA_END {
            log::error!("expected directory data end! found `{}`", dir_end_magic);
            return Err(Error::new(ErrorKind::InvalidData, "error 332"));
        }
        break;
    }

    Ok(())
}

fn file_handler(file: &mut File) -> Result<()> {
    let file_metadata = read_file_metadata(file);
    log::info!("Unpacking File `{}`", file_metadata.path);

    if read_file_u16(file) != FILE_DATA_BEGIN {
        log::error!("expected file data begin!");
        return Err(Error::new(ErrorKind::InvalidData, "error 331"));
    }

    log::info!("copying file data");

    if let Some(parent) = Path::new(&file_metadata.path).parent() {
        std::fs::create_dir_all(parent)?;
    }

    let mut new_file = File::create_new(file_metadata.path)?;
    let mut file_size = file_metadata.fsize;
    let mut data_chunk: Vec<u8> = vec![0u8; 52428800];

    while file_size > 0 {
        if file_size < 52428800 {
            data_chunk.truncate(file_size as usize);
        }
        let bytes_read = file.read(&mut data_chunk)?;
        new_file.write(&data_chunk)?;
        file_size -= bytes_read as u32;

        let fmt_size = human_data(file_size as u64);
        print!("{fmt_size} left!\r");
        std::io::stdout().flush().unwrap();
    }

    if read_file_u16(file) != FILE_DATA_END {
        log::error!("expected file data end");
        return Err(Error::new(ErrorKind::InvalidData, "error 330"));
    }

    // make the call
    type_handler(file)?;

    Ok(())
}
