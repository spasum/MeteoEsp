<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <link rel="stylesheet" href="styles/styles.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/jquery.dropdown.min.css" type="text/css" />
    <link rel="stylesheet" href="styles/bootstrap-theme.min.css" type="text/css" />
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js" type="text/javascript"></script>
    <script src="http://code.highcharts.com/highcharts.js" type="text/javascript"></script>
    <script src="scripts/CommonUtil.js" type="text/javascript"></script>
    <script src="scripts/queryHelper.js" type="text/javascript"></script>
    <script src="scripts/chartsController.js" type="text/javascript"></script>
    <script src="scripts/bootstrap.min.js" type="text/javascript"></script>
    <script src="scripts/dateFormat.min.js" type="text/javascript"></script>
    <script type="text/javascript" src="scripts/jquery.dropdown.min.js"></script>
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
                <li><a href="/">Главная</a></li>
                <li><a href="/datas.php">Данные</a></li>
                <li class="active"><a href="/charts.php">Графики</a></li>
            </ul>
            <ul class="nav navbar-nav navbar-right">
                <li><a href="/setup.php">Настройки</a></li>
            </ul>
        </div>
    </div>
</nav>

<div class="pageContainer">
    <div class="verticalPanel" style="width: 250px;">

        <div id="moduleController"></div>
        <div id="jq-dropdown-1" class="jq-dropdown jq-dropdown-tip">
            <ul class="jq-dropdown-menu" id="modulesMenu">
            </ul>
        </div>

        <hr/>

        <div id="chartController"></div>
        <div id="jq-dropdown-2" class="jq-dropdown jq-dropdown-tip">
            <ul class="jq-dropdown-menu">
                <li><a class="intervalItem" interval="1 HOUR">1 час</a></li>
                <li><a class="intervalItem" interval="2 HOUR">2 часа</a></li>
                <li><a class="intervalItem" interval="4 HOUR">4 часа</a></li>
                <li><a class="intervalItem" interval="6 HOUR">6 часов</a></li>
                <li><a class="intervalItem" interval="12 HOUR">12 часов</a></li>
                <li><a class="intervalItem" interval="1 DAY">1 день</a></li>
            </ul>
        </div>

        <hr/>

        <ul class="list-group" id="sensorsList"></ul>

    </div>
    <div class="verticalPanel">
        <div id="chart" class="chartArea"></div>
    </div>
</div>

<script type="text/javascript">
    var chartsPage;
    $(document).ready(function() {
        chartsPage = chartsController();
    });
</script>

</body>
</html>