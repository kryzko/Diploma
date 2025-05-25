const express = require('express');
const path = require('path');

const app = express();
const PORT = 3000;

// Обработка GET-запроса на корень
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname,  'control.html'));
});

// Запуск сервера
app.listen(PORT, () => {
    console.log(`Сервер запущен на http://localhost:${PORT}`);
});
