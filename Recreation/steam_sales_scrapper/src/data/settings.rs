use std::vec;
use serde::{Deserialize, Serialize};
use serde_json::Result;
use serde_json::{Value, json};
use std::path::Path;
use std::fs::{File, write, read_to_string, metadata};

use crate::data::json;

static file_path : &str = "./config.json";

pub fn get_available_stores() -> Vec<String> {
    return vec![
        String::from("steam"),
        String::from("gog"),
    ];
}

fn get_path() -> String{
    let path_str = json::get_path(file_path);
    let path = Path::new(&path_str);
    match metadata(&path_str){
        Ok(md) => {
            if md.len() == 0 {
                let settings = json!({"selected_stores": []});
                let settings_str = serde_json::to_string(&settings);
                json::write_to_file(file_path.to_string(), settings_str.expect("Initial settings could not be created.")); 
            }
        },
        Err(e) => eprintln!("Error: {}", e)
    }
    return path_str;
}

pub fn load_data() -> Result<Value> {
    let filepath = get_path();
    let data = read_to_string(filepath).unwrap();
    let body : Value = serde_json::from_str(&data)?;
    Ok(body)
}

pub fn get_selected_stores() -> Vec<String> {
    let filepath = get_path();
    let mut stores : Vec<String> = Vec::new();
    let data = read_to_string(filepath).unwrap();
    let body : Value = serde_json::from_str(&data).expect("Could convert to JSON");
    let selected = serde_json::to_string(&body["selected_stores"]).unwrap();
    //println!("{:?}", body["selected_stores"]);
    match serde_json::from_str::<Vec<String>>(&selected){
        Ok(data) => stores = data,
        Err(e) => eprintln!("Error: {}", e)
    };
    return stores;
}

pub fn update_selected_stores(selected: Vec<String>) {
    match load_data(){
        Ok(data) => {
            //println!("{:?}", data);
            let mut settings = data;
            *settings.get_mut("selected_stores").unwrap() = json!(selected);
            //println!("{:?}", settings["selected_stores"]);
            let settings_str = serde_json::to_string(&settings);
            json::write_to_file(get_path(), settings_str.expect("Cannot update store search settings"));
        },
        Err(e) => eprintln!("Error: {}", e)
    }
}

pub fn list_selected(){
    let available_stores = get_available_stores();
    let selected = get_selected_stores();
    println!("Selected Stores");
    for a_store in available_stores.iter(){
        let mut is_selected = false;
        for s_store in selected.iter() {
            if a_store == s_store {
                is_selected = true;
                break;
            }
        }
        if is_selected { println!("  [X] {}", a_store); }
        else { println!("  [ ] {}", a_store); }
    }
}