use dotenv::dotenv;
use serde_json::{Result, Value, Error};
use serde::{Deserialize, Serialize};
use std::path::Path;
use std::fs::{File, write, read_to_string};
use regex::Regex;
use std::io;
use std::io::Write;

// Structs
#[derive(Deserialize, Serialize, Debug)]
pub struct App{
    #[serde(rename = "appid")]
    pub app_id: usize,
    pub name: String,
}

#[derive(Deserialize, Serialize, Debug)]
struct Threshold{
    name: String,
    #[serde(rename="appid")]
    app_id: usize,
    currency: String,
    desired_price: f64,
}

pub struct PriceOverview{
    pub currency: String,
    pub discount_percent: usize,
    pub initial: f64,
    pub final_price: f64,
}

// Secrets
fn get_api_key() -> String {
    dotenv().ok();
    let steam_api_token = std::env::var("STEAM_API_KEY").expect("STEAM_API_KEY must be set");
    return steam_api_token;
}

// Caching Functrions
fn get_cache_path() -> String{
    let cache_path_str = "./steam_game_titles_cache.json";
    let path = Path::new(cache_path_str);
    let mut cache_fp = String::from("");
    if !path.is_file(){
        File::create_new(cache_path_str).expect("Failed to create cache file");
        println!("File created: {}", cache_fp);
        cache_fp = path.display().to_string();
    } 
    else{
        cache_fp = path.display().to_string();
    }
    return cache_fp;
}

pub async fn load_cached_games() -> Result<Vec<App>> {
    let filepath = get_cache_path();
    let data = read_to_string(filepath).unwrap();
    let temp = serde_json::from_str::<Vec<App>>(&data);
    return temp;
}

pub async fn update_cached_games(){
    let mut games_list : Vec<App> = Vec::new();
    match load_cached_games().await{
        Ok(data) => games_list = data,
        Err(e) => println!("No cached data. {}", e)
    }
    let mut temp : Vec<App> = Vec::new();
    match get_all_games().await {
        Ok(success) => {
            println!("Updating cached game titles (this will take a while)...");
            let body : Value = serde_json::from_str(&success).expect("Could convert to JSON");
            let app_list = serde_json::to_string(&body["applist"]["apps"]).unwrap();
            let data = serde_json::from_str::<Vec<App>>(&app_list);
            temp = data.unwrap();
        }, 
        Err(e) => {
            println!("Error: {}", e);
        }
    }
    for game in temp.iter() {
        let mut unique = true;
        for cached_game in games_list.iter() {
            if game.app_id == cached_game.app_id && game.name != "" {
                unique = false;
                break;
            }
        }
        if unique && game.name != "".to_string() {
            games_list.push(App {
                name: game.name.clone(),
                app_id: game.app_id.clone()
            });
        }
    }
    let data_str = serde_json::to_string(&games_list).unwrap();
    write_to_file(get_cache_path(), data_str);
    println!("Cache update complete")
}

// API Functions 
async fn get_all_games() -> Result<String> {
    let http_client = reqwest::Client::new();
    let steam_key = get_api_key();
    let format = "json";
    let url = format!("https://api.steampowered.com/ISteamApps/GetAppList/v2/?key={}&format={}",steam_key, format);
    let resp = http_client.get(url)
        .send()
        .await
        .expect("Failed to get response")
        .text()
        .await
        .expect("Failed to get data");
    Ok(resp)
}

async fn get_game_data(app_id : usize) -> Result<String>{
    let http_client = reqwest::Client::new();
    let filters = "basic,price_overview";
    let url = format!("https://store.steampowered.com/api/appdetails?appids={}&filters={}", app_id, filters);
    let resp = http_client.get(url)
        .send()
        .await
        .expect("Failed to get response")
        .text()
        .await
        .expect("Failed to get data");
    Ok(resp)
}

pub async fn get_price(app_id : usize) -> Result<PriceOverview>{
    let mut overview = PriceOverview {
        currency: String::from(""), 
        discount_percent: 0, 
        initial: 0.0, 
        final_price: 0.0,
    };
    match get_game_data(app_id).await {
        Ok(success) => {
            let body : Value = serde_json::from_str(&success).expect("Could convert to JSON");
            let price_overview : &Value = &body[app_id.to_string()]["data"]["price_overview"];
            let data = body[app_id.to_string()]["success"].clone();
            match data{
                serde_json::Value::Bool(true) => {
                    overview.final_price = price_overview["final"].as_f64().unwrap()/100.0;
                    overview.initial = price_overview["initial"].as_f64().unwrap()/100.0;
                    overview.discount_percent = price_overview["discount_percent"].as_f64().unwrap() as usize;
                    overview.currency = price_overview["currency"].to_string();
                },
                serde_json::Value::Bool(false) => {
                    eprintln!("Error: No data available for game.");
                    std::process::exit(exitcode::DATAERR);
                },
                _ => panic!("Something strange occurred")
            }
        },
        Err(e) => {
            println!("{}", e);
        }
    }
    Ok(overview)
}

// Command Functions
pub async fn check_game(name: &str) -> Option<App> {
    let mut games_list : Vec<App> = Vec::new();
    match load_cached_games().await {
        Ok(data) => games_list = data,
        Err(e) => println!("Error: {}", e)
    }
    if games_list.len() == 0 {
        update_cached_games().await;
        match load_cached_games().await {
            Ok(data) => games_list = data,
            Err(e) => println!("Error: {}", e)
        }
    }
    for elem in games_list.iter(){
        if name.to_owned() == elem.name {
            return Ok::<App, Error>(App {
                name: name.to_owned(),
                app_id: elem.app_id
            }).ok();
        }
    }
    None
}

// Search Functions
async fn search_by_keyphrase(keyphrase: &str) -> Result<Vec<String>>{
    let mut games_list : Vec<App> = Vec::new();
    match load_cached_games().await {
        Ok(data) => games_list = data,
        Err(e) => println!("Error: {}", e)
    }
    if games_list.len() == 0 {
        update_cached_games().await;
        match load_cached_games().await {
            Ok(data) => games_list = data,
            Err(e) => println!("Error: {}", e)
        }
    }
    let mut search_list : Vec<String> = Vec::new();
    let re = Regex::new(keyphrase).unwrap();
    for game in games_list.iter(){
        let caps = re.captures(&game.name);
        if !caps.is_none() {
            search_list.push(game.name.clone());
        }
    }
    Ok(search_list)
}

pub async fn search_game(keyphrase: &str) -> Option<String>{ 
    match search_by_keyphrase(keyphrase).await {
        Ok(search_list) => {
            println!("Did you mean one of the following?");
            for (idx, game_title) in search_list.iter().enumerate() {
                println!("  [{}] {}", idx, game_title);
            }
            println!("  [q] Quit");
            let mut input = String::new();
            print!("Type integer corresponding to game title or type \"q\" to quit: ");
            let _ = io::stdout().flush();
            io::stdin()
                .read_line(&mut input)
                .expect("Failed to read user input");
            if input.trim() == "q" {
                eprintln!("Request terminated.");
            }
            else {
                match input.trim().parse::<usize>() {
                    Ok(idx) => {
                        if idx < search_list.len(){
                            let title = search_list[idx].clone();
                            return Ok::<std::string::String, Error>(title).ok();
                        }
                        else if idx >= search_list.len(){
                            eprintln!("Integer \"{}\" is invalid. Request terminated.", idx);
                        }
                    },
                    Err(e) => println!("Invalid input: {}\nError: {}", input, e)
                }
            }
        }, 
        Err(e) => println!("Error: {}", e)
    }
    None
}

// Helper Function
fn write_to_file(path: String, data: String){
    write(path, data).expect("Data could not be saved.");
}
