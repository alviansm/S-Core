# EEShip - Energy Efficient Ship

**Team Name:** Tim A

**Application Type:** Desktop App + Web App (REST API)

**Project Repository:** 
1. Desktop App: https://repo.sucofindo.co.id/team.a/eeship-desktop-app
2. Server App:

## 1. Application Overview

EEShip is a holistic IoT & AI system for ship voyage planning and monitoring, supporting IMO standards towards net-zero carbon targets on existing vessels.

EEShip merupakan bagian dasar dari Project S yang diusulkan dalam proposal Hackinfest 2025.

## 2. System Requirements

### Hardware Requirements

#### Minimum Specifications:
- **Operating System:** Windows 10 (Version 1809 or newer)
- **Processor (CPU):** Dual-Core, 1.0 GHz or faster
- **Memory (RAM):** 4 GB RAM
- **Storage:** Minimum 1 GB free space on SSD/HDD
- **Graphics Card:** Integrated graphics supporting DirectX 9
- **Screen Resolution:** 1024 x 768 pixels
- **Network:** Internet connection (if required)

#### Recommended Specifications:
- **Operating System:** Windows 11
- **Processor (CPU):** Quad-Core, 2.5 GHz or faster (e.g., Core i5 Gen 8+, AMD Ryzen 5+)
- **Memory (RAM):** 8 GB RAM
- **Storage:** 256 GB or more SSD (Solid State Drive)
- **Graphics Card:** Modern Integrated Graphics (Intel Iris Xe/AMD Radeon) or Dedicated GPU (2 GB VRAM)
- **Screen Resolution:** 1920 x 1080 (Full HD)
- **Network:** Broadband internet connection (min. 10 Mbps)

### Software Requirements

#### Desktop App:
- **Operating System:** Windows
- **Programming Language/Framework:** Qt 6.8.3 (C++)
- **Libraries/Dependencies:**
  1. Qt6/C++ Widgets
  2. Qt Advanced Docking System ([GitHub](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System))

#### Server API:
- **Operating System:** Windows / MacOS / Linux
- **Programming Language/Framework:** Golang 1.24
- **Database:** MySQL
- **Libraries/Dependencies:**
  1. Gin-gonic ([GitHub](https://github.com/gin-gonic))
  2. Gorm ([GitHub](https://github.com/go-gorm/gorm))

## 3. Installation Prerequisites

### Desktop App:
- Qt 6.8.3 (qt.io)
- Qt Creator
- MSVC2022 Compiler (Visual Studio 2022)
- CMake (Minimum Version 3.16)

### Server API:
- Golang 1.24 or higher
- MySQL

## 4. Installation Steps

### Desktop App:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/namarepo.git
   ```

2. **Open the project folder:**
   - Open the project folder using Qt Creator application
   - Select CMakeLists.txt to open the project in Qt Creator

3. **Install dependencies:**
   - Configure compiler using MSVC2022

4. **Run the application:**
   - Press the "Start" button or Ctrl+R in Qt Creator

**Note:** The team has prepared an .exe installer so you don't need to build or install the environment yourself.

### Server API:

1. **Clone the repository:**
   ```bash
   git clone https://github.com/namarepo.git
   ```

2. **Navigate to project folder:**
   ```bash
   cd nama-project
   ```

3. **Install dependencies:**
   ```bash
   go mod tidy
   ```

4. **Configure Database:**
   - Set up localhost or MySQL Server

5. **Prepare .env file with the following variables:**
   ```env
   DATABASE_URL=url_database (Example: localhost)
   DATABASE_PORT=3306
   DATABASE_USER=user_mysql
   DATABASE_PASS=password_mysql
   DATABASE_NAME=score
   PORT=3024
   ```

## 5. Running the Application

### Desktop App:
The application will open automatically in a new window.

### Server API:
```bash
go run build
```

The server will run on the port specified in .env file.

**Example:** `localhost:3024`

**API Documentation:** [https://documenter.getpostman.com/view/20809206/2sB3QCSDmK](https://documenter.getpostman.com/view/20809206/2sB3QCSDmK)

## 6. Troubleshooting

### Internal Memory
Qt6 consumes significant internal memory due to comprehensive packages and sources. Safe storage for Qt installation is around 50 GB.

### Compiler Version
Use a modern C++ compiler (minimum C++17). During desktop app development, the team used MSVC2022 64-bit and Qt version 6.8.3.

### Incomplete Dependencies
Qt consists of different packages. Ensure all dependencies are installed (can be viewed in CMakeLists.txt), including QtPdf and QtChart.

## 7. User Manual

### EEShip Desktop

EEShip desktop is designed to operate even without continuous signal connection, ensuring uninterrupted ship operations. The EEShip interface is generally divided into:

1. **Menu Bar:** A horizontal strip at the top of the application window displaying a list of command menus such as File, Edit, and Help, allowing users to access and execute various program functions by clicking on the menus.

2. **Ribbon:** Action buttons at the application level.

3. **Sidebar:** Used for navigation and opening or closing application pages.

4. **Page Toolbar:** List of buttons related to the page.

5. **Active Page**

#### Prototype Pages:
- **Welcome:** Initial landing page
- **Dashboard:** Main overview dashboard
- **Technical:** Technical specifications and data
- **Decision Support:** AI-powered decision assistance
- **Voyage Planning:** Route planning and optimization
- **History & Reporting:** Historical data and reports

**Note:** During the prototype development phase, unimplemented functionalities will be indicated by the application through message boxes or dialogs explaining the feature.

### EEShip Server

Used as a proof of concept for sensor reading instrumentation or actuators from the IoT system that will be developed in the future.