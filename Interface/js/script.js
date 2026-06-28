let initialLoaded = false;
let initialState = null;
let solving = false;

const URL_BASE = "https://defensive-elaine-ifpr-d62571ec.koyeb.app"

const initialBoard = document.getElementById("initialBoard");
const solutionBoard = document.getElementById("solutionBoard");

const solveBtnHill = document.getElementById("solveBtnHill");
const solveBtnA = document.getElementById("solveBtnA");
const iniciarEstadosBtn = document.getElementById("iniciarEstados");

createBoard(initialBoard);
createBoard(solutionBoard);

async function requestEstadoInicial() {
    const response = await fetch(`${URL_BASE}/estado-inicial/8`);

    if (!response.ok) {
        throw new Error(`Erro HTTP ao gerar estado inicial: ${response.status}`);
    }

    return await response.json();
}

async function requestHill(estado) {

    const response = await fetch(`${URL_BASE}/hill-climbing/8`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ estado_inicial: estado }) // Envia o vetor como JSON
    });

    if (!response.ok) {
        throw new Error(`Erro HTTP no Hill Climbing: ${response.status}`);
    }

    return await response.json();
}

async function requestA(estado) {

    const response = await fetch(`${URL_BASE}/a-estrela/8`, {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ estado_inicial: estado }) // Envia o vetor como JSON
    });

    if (!response.ok) {
        throw new Error(`Erro HTTP no A*: ${response.status}`);
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

        const resultado = await requestEstadoInicial();

        //equalidade estita
        if (resultado.status === "sucesso") {
            initialState = resultado.estado_inicial;
            initialLoaded = true;
            renderQueens(initialState, initialBoard);
        } else {
            console.log("Erro da API: " + resultado.mensagem);
        }

     
        //limpa métricas e solução antiga
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

    if (!initialLoaded || !initialState ) {
        alert("Primeiro clique em 'Iniciar Estados'.");
        return;
    }

    if (solving) return;

    solving = true;
    setButtonsEnabled(false);

    try {

        clearQueens(solutionBoard);

        //passa o estado inicial gerado pelo botao como parametro
        const resultado = await requestHill(initialState);

        if (resultado.status === "sucesso") {
            renderQueens(resultado.solucao, solutionBoard);
            renderMetrics(resultado, "hill-climbing");
        } else {
            alert("Erro no algoritmo: " + resultado.mensagem);
        }

    } catch (erro) {

        console.error("Erro ao obter solução:", erro);

    } finally {

        solving = false;
        setButtonsEnabled(true);
    }

});

solveBtnA.addEventListener("click", async () => {

    if (!initialLoaded || !initialState) {
        alert("Primeiro clique em 'Iniciar Estados'.");
        return;
    }

    if (solving) return;

    solving = true;
    setButtonsEnabled(false);

    try {

        clearQueens(solutionBoard);

        //pega o estado inicial de entrada
        const resultado = await requestA(initialState);

        if (resultado.status === "sucesso") {
            renderQueens(resultado.solucao, solutionBoard);
            renderMetrics(resultado, "a-estrela");
        } else {
            alert("Erro no algoritmo: " + resultado.mensagem);
        }

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
        
    if (tipoAlgoritmo === "a-estrela") {
        document.getElementById("reinicios").textContent = resultado.reinicios;
    } else {
        document.getElementById("reinicios").textContent = resultado.metricas.reinicios;
    }
}