let initialLoaded = false;
let initialState = null;
let solving = false;

const initialBoard = document.getElementById("initialBoard");
const solutionBoard = document.getElementById("solutionBoard");

const solveBtnHill = document.getElementById("solveBtnHill");
const solveBtnA = document.getElementById("solveBtnA");
const iniciarEstadosBtn = document.getElementById("iniciarEstados");

createBoard(initialBoard);
createBoard(solutionBoard);

async function requestHill() {

    const response = await fetch(
        "https://defensive-elaine-ifpr-d62571ec.koyeb.app/hill-climbing/8"
    );

    if (!response.ok) {
        throw new Error(`Erro HTTP: ${response.status}`);
    }

    return await response.json();
}

async function requestA() {

    const response = await fetch(
        "https://defensive-elaine-ifpr-d62571ec.koyeb.app/hill-climbing/8"
        // Troque para o endpoint do A* quando existir
    );

    if (!response.ok) {
        throw new Error(`Erro HTTP: ${response.status}`);
    }

    return await response.json();
}

function setButtonsEnabled(enabled) {
    iniciarEstadosBtn.disabled = !enabled;
    solveBtnHill.disabled = !enabled;
    solveBtnA.disabled = !enabled;
}

iniciarEstadosBtn.addEventListener("click", async () => {

    if (solving) return;

    solving = true;
    setButtonsEnabled(false);

    try {

        clearQueens(initialBoard);

        const resultado = await requestHill();

        initialState = resultado.estado_inicial;
        initialLoaded = true;

        renderQueens(initialState, initialBoard);

        // Limpa métricas e solução antiga
        clearQueens(solutionBoard);

    } catch (erro) {

        console.error("Erro ao carregar estados iniciais:", erro);
        alert("Erro ao carregar estados iniciais.");

    } finally {

        solving = false;
        setButtonsEnabled(true);
    }

});

solveBtnHill.addEventListener("click", async () => {

    if (!initialLoaded) {
        alert("Primeiro clique em 'Iniciar Estados'.");
        return;
    }

    if (solving) return;

    solving = true;
    setButtonsEnabled(false);

    try {

        clearQueens(solutionBoard);

        const resultado = await requestHill();

        renderQueens(resultado.solucao, solutionBoard);
        renderMetrics(resultado);

    } catch (erro) {

        console.error("Erro ao obter solução:", erro);

    } finally {

        solving = false;
        setButtonsEnabled(true);
    }

});

solveBtnA.addEventListener("click", async () => {

    if (!initialLoaded) {
        alert("Primeiro clique em 'Iniciar Estados'.");
        return;
    }

    if (solving) return;

    solving = true;
    setButtonsEnabled(false);

    try {

        clearQueens(solutionBoard);

        const resultado = await requestA();

        renderQueens(resultado.solucao, solutionBoard);
        renderMetrics(resultado);

    } catch (erro) {

        console.error("Erro ao obter solução:", erro);

    } finally {

        solving = false;
        setButtonsEnabled(true);
    }

});

function createBoard(boardElement) {

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

            boardElement.appendChild(cell);
        }
    }
}

function renderQueens(solution, boardElement) {

    solution.forEach((col, row) => {

        const cell = boardElement.querySelector(
            `[data-row="${row}"][data-col="${col}"]`
        );

        if (!cell) return;

        const img = document.createElement("img");

        img.src = "assets/queen.png";
        img.alt = "Rainha";
        img.classList.add("queen");

        cell.appendChild(img);
    });
}

function clearQueens(boardElement) {

    if (!boardElement) return;

    boardElement.querySelectorAll(".queen").forEach(q => q.remove());
}

function renderMetrics(resultado) {

    document.getElementById("status").textContent =
        resultado.status;

    document.getElementById("tempoExecucao").textContent =
        `${resultado.metricas.tempo_execucao_ms} ms`;

    document.getElementById("reinicios").textContent =
        resultado.metricas.reinicios;

    document.getElementById("nosExpandidos").textContent =
        resultado.metricas.nos_expandidos;

    document.getElementById("nosGerados").textContent =
        resultado.metricas.nos_gerados;

    document.getElementById("nRainhas").textContent =
        resultado.n_rainhas;
}