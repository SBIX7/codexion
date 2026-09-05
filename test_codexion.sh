#!/bin/bash

LOG_FILE="codexion_deep_test.log"
> "$LOG_FILE" # Vide le fichier au démarrage

echo "Démarrage de la suite de tests ultra-rigide. Patientez, Valgrind/Helgrind ralentissent l'exécution..."
echo "Les résultats sont enregistrés en direct dans $LOG_FILE"

echo "=== COMPILATION ===" >> "$LOG_FILE"
make re >> "$LOG_FILE" 2>&1
echo -e "\n\n" >> "$LOG_FILE"

# Fonction de test globale
run_rigorous_test() {
    local base_args=$1
    local scheduler=$2
    local desc=$3

    echo "===============================================================================" | tee -a "$LOG_FILE"
    echo "TEST : ./codexion $base_args $scheduler" | tee -a "$LOG_FILE"
    echo "ATTENTE : $desc" | tee -a "$LOG_FILE"
    echo "===============================================================================" | tee -a "$LOG_FILE"

    echo ">>> EXÉCUTION STANDARD" >> "$LOG_FILE"
    ./codexion $base_args $scheduler >> "$LOG_FILE" 2>&1
    echo -e "\n" >> "$LOG_FILE"

    echo ">>> EXÉCUTION VALGRIND (MEMORY LEAKS)" >> "$LOG_FILE"
    valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all ./codexion $base_args $scheduler >> "$LOG_FILE" 2>&1
    echo -e "\n" >> "$LOG_FILE"

    echo ">>> EXÉCUTION HELGRIND (DATA RACES & DEADLOCKS)" >> "$LOG_FILE"
    valgrind --tool=helgrind ./codexion $base_args $scheduler >> "$LOG_FILE" 2>&1
    echo -e "\n\n" >> "$LOG_FILE"
}

# 1. Test du codeur unique (Burnout)[cite: 5]
run_rigorous_test "1 800 200 200 200 10 0" "fifo" "Codeur unique, doit mourir vers 800ms"
run_rigorous_test "1 800 200 200 200 10 0" "edf" "Codeur unique, doit mourir vers 800ms"

# 2. Test standard sans burnout (Cycle faisable)[cite: 5]
run_rigorous_test "5 2000 200 200 200 5 0" "fifo" "5 codeurs, temps large (2000ms), 5 compils, 0 cooldown. Aucun burnout."
run_rigorous_test "5 2000 200 200 200 5 0" "edf" "5 codeurs, temps large (2000ms), 5 compils, 0 cooldown. Aucun burnout."

# 3. Test de Burnout inévitable[cite: 5]
run_rigorous_test "5 500 200 200 200 10 0" "fifo" "Cycle de 600ms > burnout 500ms. Mort obligatoire."
run_rigorous_test "5 500 200 200 200 10 0" "edf" "Cycle de 600ms > burnout 500ms. Mort obligatoire."

# 4. Test du Cooldown modéré[cite: 5]
run_rigorous_test "5 3000 200 200 200 5 400" "fifo" "Cooldown de 400ms. Aucun burnout, dongles bloqués 400ms."
run_rigorous_test "5 3000 200 200 200 5 400" "edf" "Cooldown de 400ms. Aucun burnout, dongles bloqués 400ms."

# 5. Test de Forte Contention[cite: 5]
run_rigorous_test "5 3000 200 200 200 5 800" "fifo" "Forte contention (800ms cooldown). Ordre strict FIFO."
run_rigorous_test "5 3000 200 200 200 5 800" "edf" "Forte contention (800ms cooldown). Ordonnancement par deadline EDF."

echo "Tests terminés ! Ouvre le fichier $LOG_FILE ou envoie-moi son contenu."
