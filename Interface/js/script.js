    let initialLoaded = false;
    let initialState = null;

    const initialBoard = document.getElementById("initialBoard");
    const solutionBoard = document.getElementById("solutionBoard");

    const solveBtnHill = document.getElementById("solveBtnHill");
    const solveBtnA = document.getElementById("solveBtnA");
    const iniciarEstadosBtn = document.getElementById("iniciarEstados")

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
        );

        if (!response.ok) {
            throw new Error(`Erro HTTP: ${response.status}`);
        }

        return await response.json();
    }

    iniciarEstadosBtn.addEventListener("click", async () => {
        try {
            clearQueens(initialBoard);

            const resultado = await requestHill();

            initialState = resultado.estado_inicial;
            initialLoaded = true;

            console.log("Estado inicial carregado:", initialState);

            renderQueens(initialState, initialBoard);

        } catch (erro) {
            console.error("Erro ao carregar estados iniciais:", erro);
            alert("Erro ao carregar estados iniciais.");
        }
    });

    solveBtnHill.addEventListener("click", async () => {

        if (!initialLoaded) {
            alert("Primeiro clique em 'Iniciar Estados'.");
            return;
        }

        clearQueens(solutionBoard);

        try {
            const resultado = await requestHill();

            console.log("Resposta da API:", resultado);

            renderQueens(resultado.solucao, solutionBoard);

        } catch (erro) {
            console.error("Erro ao obter solução:", erro);
        }
    });

    solveBtnA.addEventListener("click", async () => {

        if (!initialLoaded) {
            alert("Primeiro clique em 'Iniciar Estados'.");
            return;
        }

        clearQueens(solutionBoard);

        try {
            const resultado = await requestA();

            console.log("Resposta da API:", resultado);

            renderQueens(resultado.solucao, solutionBoard);

        } catch (erro) {
            console.error("Erro ao obter solução:", erro);
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

        boardElement
            .querySelectorAll(".queen")
            .forEach(q => q.remove());
    }