// plans for the future
//  * make file insert function
//  * make file reader function

pub fn human_data(bytes: u64) -> String {
    let mut format;
    format = "Bytes".to_string();
    let mut bytes: f32 = bytes as f32;

    if bytes > 1024.0 {
        bytes /= 1024.0;
        format = "KiB".to_string();
    }
    if bytes > 1024.0 {
        bytes /= 1024.0;
        format = "MiB".to_string();
    }
    if bytes > 1024.0 {
        bytes /= 1024.0;
        format = "GiB".to_string();
    }
    if bytes > 1024.0 {
        bytes /= 1024.0;
        format = "TiB".to_string();
    }

    return format!("{} {}", bytes, format);
}
