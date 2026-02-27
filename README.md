# Iron Man Reactor Clock

## Overview
The Iron Man Reactor Clock is a project designed to showcase the functionality of a digital clock inspired by the Iron Man arc reactor. The clock displays the current time in a visually appealing manner, replicating the aesthetics of Tony Stark's technology.

## Features
- **Real-time Clock:** Displays the current time and updates every second.
- **Customizable Design:** Users can change the appearance and layout of the clock.
- **User-friendly Interface:** Intuitive controls for easy setup and configuration.
- **Multiple Time Zones:** Ability to display time in various time zones.
- **Alarm Functionality:** Set alarms for important reminders.

## Setup Instructions
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/helmesj/IronManReactorClock.git
   ```
2. **Navigate to the Project Directory:**
   ```bash
   cd IronManReactorClock
   ```
3. **Install Dependencies:**
   Follow the installation instructions below for your specific platform.

## Installation
### For Windows:
- Ensure you have [Node.js](https://nodejs.org/) installed.
- Run the following command in the project directory:
   ```bash
   npm install
   ```
### For macOS/Linux:
- Make sure you have [Node.js](https://nodejs.org/) installed.
- Use the following command in the terminal:
   ```bash
   npm install
   ```

## Configuration
- Create a configuration file named `config.json` in the root directory with the following structure:
   ```json
   {
     "timeZone": "UTC",
     "alarm": "07:00"
   }
   ```
- Modify the `timeZone` field to your desired time zone and set the `alarm` to your preferred alarm time.

## Usage
1. Start the application:
   ```bash
   npm start
   ```
2. Open your web browser and navigate to `http://localhost:3000`
3. You will see the Iron Man Reactor Clock displayed on your screen.
4. Use the interface to set alarms or change settings.

## Troubleshooting
- **Clock not updating:** Ensure that the application is running and that you have a stable internet connection.
- **Errors during installation:** Check if Node.js is properly installed and your environment variables are set correctly.
- **Configuration issues:** Verify the `config.json` structure and ensure it adheres to JSON standards.

For further assistance, check the [issues section](https://github.com/helmesj/IronManReactorClock/issues) of the repository.