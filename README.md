# 🧩 JeuPuzzle

**JeuPuzzle** is a desktop puzzle game developed in C++ using the Qt framework. It offers an engaging experience where players can solve image-based puzzles, enhancing their problem-solving skills and providing entertainment.

## 🎮 Features

- **Interactive Gameplay**: Drag and drop puzzle pieces to complete the image.
- **Multiple Languages**: Supports English and Malagasy, catering to a diverse user base.
- **Customizable Puzzles**: Easily add new images to create different puzzle challenges.
- **User-Friendly Interface**: Intuitive design ensures a smooth user experience.

## 🛠️ Technologies Used

- **Programming Language**: C++
- **Framework**: Qt
- **Build System**: QMake

## 📁 Project Structure

- `main.cpp`: Entry point of the application.
- `mainwindow.cpp` & `mainwindow.h`: Define the main window and its functionalities.
- `puzzle.cpp` & `puzzle.h`: Handle puzzle logic and operations.
- `piecemodel.cpp` & `piecemodel.h`: Manage individual puzzle pieces.
- `image.qrc`: Resource file containing images used in the game.
- `puzzle_en.ts` & `puzzle_mg.ts`: Translation files for English and Malagasy languages.
- `moi.jpeg`: Sample image used for the puzzle.

## 🚀 Getting Started

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Herman-Riah19/JeuPuzzle.git
   ```

2. **Open the Project**:
   - Use Qt Creator to open the `Puzzle.pro` file.

3. **Build and Run**:
   - Configure the build settings as needed.
   - Build the project and run the application to start playing.

## 🌐 Localization

The game supports multiple languages. To add more translations:

1. Create a new `.ts` file for the desired language.
2. Use Qt Linguist to provide translations.
3. Update the resource files and recompile the application.

## 📄 License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
