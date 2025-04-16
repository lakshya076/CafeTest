# CafeTest
A Cafe Management System built using C++, SQL and the Qt Framework.

## Features

- Cross-platform GUI with Qt Widgets/QML
- Responsive, modern and user-friendly interface
  
- ### **User Interface**
  - Home Page
    - Contains dishes added by the admin from the [CafeInterface](#Cafe-Interface)
    - Each dish card contains an add to cart button and incrementer/decrementer
    - Every dish has an available quantity
    - Vegetarian Indicators for each dish
    - Dish of The Day Column on the right side of the home page
    - Dining/Takeaway options
    - Everything is loaded from a database (cafe.db) using Qt SQLite plugin
  - Feedback Page
  - Settings Page
 
- ### **Cafe Interface**
  - Add Items
    - Window to add new dishes to the database which can be loaded in the home page
  - Delete Items
    - Window to delete the current dish(es) in the database and the changes will be reflected in the home page
  - Dish Of the Day
    - Window to update the price and available quantity of the dish of the day
    - Dish of the Day is pre defined and only their price and quantity can be updated. They cannot be deleted
  - Update Items
    - Window to update the price and quantity of a chosen dish.


## Technologies Used

- **Language:** C++
- **Framework:** Qt (version 6.8.3)
- **Build System:** qmake
- **Database:** SQLite


## Develop/Implement

### Prerequisites

- Qt SDK installed ([Download here](https://www.qt.io/download))
- Qt Creator
- C++ compiler (e.g., g++, MSVC)
- Add `C:\Qt\X.X.X\mingw_64\bin` to Path (X.X.X will be system specific)

### Build Instructions (qmake)
- Clone the repository using either [GitHub Desktop](https://github.com/apps/desktop?ref_cta=download+desktop&ref_loc=installing+github+desktop&ref_page=docs) or
  
  ```git
  git clone https://github.com/lakshya076/CafeTest.git
  ```
- Open the `CafeTest.pro` file in Qt Creator
- Build and Run the project using `Ctrl+R`

## Contributing
- [Pull Requests](https://github.com/lakshya076/CafeTest/pulls) are welcome but for major changes or improvements, please open an [issue](https://github.com/lakshya076/CafeTest/issues) first to discuss the changes.

## Preview
Coming Soon
