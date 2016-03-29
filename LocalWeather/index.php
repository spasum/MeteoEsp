<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <link rel="stylesheet" href="styles/styles.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap-theme.min.css" type="text/css" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js" type="text/javascript"></script>
    <script src="scripts/CommonUtil.js" type="text/javascript"></script>
    <script src="scripts/queryHelper.js" type="text/javascript"></script>
    <script src="scripts/indexController.js" type="text/javascript"></script>
    <script src="scripts/bootstrap.min.js" type="text/javascript"></script>
    <script type="text/javascript">
        var timerStart = Date.now();
    </script>
</head>
<body>

<nav class="navbar navbar-default">
    <div class="container-fluid">
        <div class="navbar-header">
            <a class="navbar-brand" href="/">Домашняя метеостанция</a>
        </div>
        <div class="collapse navbar-collapse" id="bs-example-navbar-collapse-1">
            <ul class="nav navbar-nav">
                <li class="active"><a href="/">Главная</a></li>
                <li><a href="/datas.php">Данные</a></li>
                <li><a href="/charts.php">Графики</a></li>
            </ul>
            <ul class="nav navbar-nav navbar-right">
                <li><a href="/setup.php">Настройки</a></li>
            </ul>
        </div>
    </div>
</nav>

<div class="pageContainer" id="pageContainer"></div>

<script type="text/javascript">
    var indexPage;
    $(document).ready(function() {
        indexPage = indexController();
    });
</script>

</body>
</html>