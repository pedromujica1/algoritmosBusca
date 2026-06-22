const board = document.getElementById("board");
const solveBtn = document.getElementById("solveBtn");

createBoard();
let queens = [
    [0, 0],
    [1, 4],
    [2, 7],
    [3, 5],
    [4, 2],
    [5, 6],
    [6, 1],
    [7, 3]
];

renderQueens(queens);

solveBtn.addEventListener("click", () => {

    clearQueens();

    /*
        Aqui futuramente:
        fetch('/api/solve')
        .then(...)
    */

    renderQueens(queens);
});

function createBoard() {

    for (let row = 0; row < 8; row++) {

        for (let col = 0; col < 8; col++) {

            const cell = document.createElement("div");

            cell.classList.add("cell");

            if ((row + col) % 2 === 0) {
                cell.classList.add("white");
            } else {
                cell.classList.add("black");
            }

            cell.dataset.row = row;
            cell.dataset.col = col;

            board.appendChild(cell);
        }
    }
}

function renderQueens(positions) {

    positions.forEach(([row, col]) => {

        const cell = document.querySelector(
            `[data-row="${row}"][data-col="${col}"]`
        );

        const img = document.createElement("img");

        img.src = "assets/queen.png";
        img.alt = "Rainha";
        img.classList.add("queen");

        cell.appendChild(img);
    });
}

function clearQueens() {
    document
        .querySelectorAll(".queen")
        .forEach(q => q.remove());
}