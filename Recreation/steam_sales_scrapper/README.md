# Steam Sales Scrapper
The purpose of this script is to scrape the Steam Web API to determine whether a game has reached a specified price threshold. If one or more games fall below the user-defined limit an email will be sent containing a list of games along with their respective prices. 

Officially tested on Ubuntu 24.04 and Windows 11.

## Quick Start
1. Setup SMTP server/service (TLS required/optional)
2. Nagivate to project folder and run `cargo build`
3. In the project folder, create `.env` with the following:
    ```
    STEAM_API_KEY={your_steam_api_key}
    RECIPIENT_EMAIL={destination_email_address}
    SMTP_HOST={smtp_host_domain}
    SMTP_PORT={port_number}
    SMTP_EMAIL={smtp_email_address}
    SMTP_USERNAME={smtp_username}
    SMTP_PWD={stmp_password}
    ```
4. Add games and their respective price threshold using the [support commands](#supported-commands) below (supports commands from cargo).
5. [Optional] Automate emails
    - **For Unix-based systems:** Update *SCHEDULE* variable to desired execution frequency and run `set_cron.sh` with root privileges.
    - **For Windows systems:** Update *$trigger* variable to desired execution frequency and run `set_task_scheduler.ps1`. 
    
        If Powershell scripts execution is not enabled run the following with administrative privileges: 
        ```
        Set-ExecutionPolicy RemoteSigned
        ```

## Supported Commands
Use the`--help` flag in command line to get more information on the supported commands. Here's a brief description and example of each command.
- `SEARCH` := find a game title based on keyphrase and add game with price threshold.
    ```bash 
    steam_sales_scrapper search --keyphrase <keyphrase>
    ```
- `ADD` := add a specified game (title must be exact to work).
    ```bash 
    steam_sales_scrapper add --title <title> --price <price>
    ```
- `UPDATE` := update price threshold for a specified game.
    ```bash 
    steam_sales_scrapper update --title <title> --price <price>
    ```
- `REMOVE` := remove a specified game.
    ```bash 
    steam_sales_scrapper remove --title <title>
    ```
- `LIST` := list all the stored price thresholds for selected games.
    ```bash 
    steam_sales_scrapper --list-all
    ```
- `CACHE` := update the locally stored cache of steam games (title and app ids).
    ```bash 
    steam_sales_scrapper --update-cache
    ```
- `EMAIL` := sends an email (using SMTP) containing a list of games that are below user defined price threshold for each game. No email is sent if no game has reached their price threshold.
    ```bash 
    steam_sales_scrapper --send-email
    ```