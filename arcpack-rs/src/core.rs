use crate::magic::*;
use std::fs::File;
use std::io::{Read, Result, Write};

pub struct ArchiveSectionMetadata {
    pub name_len: u8,
    pub path_len: u16,
    pub fsize: u32,
    pub name: String,
    pub path: String,
}

impl ArchiveSectionMetadata {
    pub fn new() -> Self {
        ArchiveSectionMetadata {
            name_len: 0,
            path_len: 0,
            fsize: 0,
            name: String::new(),
            path: String::new(),
        }
    }
}

pub fn check_file_header(archive_name: &str) -> bool {
    log::info!("checking header");
    let header: [u8; 9] = [b'A', b'r', b'c', b'P', b'a', b'c', b'k', VERSION, MAGIC];

    let mut file = File::open(archive_name).unwrap();
    let mut buffer = vec![0u8; 9];
    file.read(&mut buffer).unwrap();

    if buffer[7] != VERSION {
        log::error!("Archive version not supported!");
        return false;
    }

    for i in 0..buffer.len() {
        if header[i] != buffer[i] {
            return false;
        }
    }
    return true;
}

pub fn read_file_u16(file: &mut File) -> u16 {
    let mut bytes: [u8; 2] = [0u8; 2];
    let bread = file.read(&mut bytes).unwrap();
    if bread == 0 {
        return END_OF_FILE;
    }
    let word: u16 = ((bytes[1] as u16) << 8) | (bytes[0] as u16);
    return word;
}

pub fn read_dir_metadata(file: &mut File) -> ArchiveSectionMetadata {
    let mut metadata = ArchiveSectionMetadata::new();
    let mut single: [u8; 1] = [0u8; 1];
    log::info!("Reading Directory Metadata");

    file.read_exact(&mut single).unwrap();
    metadata.name_len = single[0];
    metadata.path_len = read_file_u16(file);

    let mut name: Vec<u8> = vec![0u8; metadata.name_len as usize];
    let mut path: Vec<u8> = vec![0u8; metadata.path_len as usize];

    file.read_exact(&mut name).unwrap();
    file.read_exact(&mut path).unwrap();

    metadata.name = String::from_utf8(name).unwrap();
    metadata.path = String::from_utf8(path).unwrap();

    return metadata;
}

pub fn read_file_metadata(file: &mut File) -> ArchiveSectionMetadata {
    let mut metadata = ArchiveSectionMetadata::new();
    let mut single: [u8; 1] = [0u8; 1];
    log::info!("Reading File metadata");

    file.read_exact(&mut single).unwrap();
    metadata.name_len = single[0];
    metadata.path_len = read_file_u16(file);

    let fsize_higher = read_file_u16(file);
    let fsize_lower = read_file_u16(file);
    if fsize_higher == END_OF_FILE || fsize_lower == END_OF_FILE {
        panic!("unexpected end of file");
    }

    metadata.fsize = ((fsize_lower as u32) << 16) | (fsize_higher as u32);

    let mut name: Vec<u8> = vec![0u8; metadata.name_len as usize];
    let mut path: Vec<u8> = vec![0u8; metadata.path_len as usize];

    file.read_exact(&mut name).unwrap();
    file.read_exact(&mut path).unwrap();

    metadata.name = String::from_utf8(name).unwrap();
    metadata.path = String::from_utf8(path).unwrap();

    return metadata;
}

pub fn write_header(file: &mut File) {
    log::info!("Writing header");
    let header: [u8; 9] = [b'A', b'r', b'c', b'P', b'a', b'c', b'k', VERSION, MAGIC];
    file.write_all(&header).unwrap();
}

pub fn write_file_u16(file: &mut File, word: u16) {
    let high: u8 = (word >> 8) as u8; // bug ?
    let low: u8 = (word & 0xFF) as u8;
    let buf: [u8; 2] = [low, high];
    file.write_all(&buf).unwrap();
}

pub fn write_file_metadata(file: &mut File, name: &str, path: &str, fsize: u32) -> Result<()> {
    log::info!("Writing file metadata");

    let name_len = name.len() as u8;
    let path_len = path.len() as u16;
    let mut fsize_u16: [u16; 2] = [0u16; 2];

    fsize_u16[0] = (fsize & 0xFFFF) as u16;
    fsize_u16[1] = (fsize >> 16) as u16;

    write_file_u16(file, FILE_BEGIN);
    file.write(&[name_len])?;
    write_file_u16(file, path_len);

    write_file_u16(file, fsize_u16[0]);
    write_file_u16(file, fsize_u16[1]);

    file.write_all(name.as_bytes())?;
    file.write_all(path.as_bytes())?;

    write_file_u16(file, FILE_DATA_BEGIN);
    Ok(())
}

pub fn write_dir_metadata(file: &mut File, name: &str, path: &str) -> Result<()> {
    log::info!("Writing dir metadata");

    let name_len: u8 = name.len() as u8;
    let path_len: u16 = path.len() as u16;

    write_file_u16(file, DIR_BEGIN);
    file.write(&[name_len])?;
    write_file_u16(file, path_len);

    file.write(name.as_bytes())?;
    file.write(path.as_bytes())?;

    write_file_u16(file, DIR_DATA_BEGIN);
    Ok(())
}
