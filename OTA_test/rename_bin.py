Import("env")

env.Replace(PROGNAME="%s" % env.GetProjectOption("custom_prog_version"))
def before_upload(source, target, env):
    # Command to erase the flash
    env.Execute("$PYTHONEXE $UPLOADER --chip esp32-s3 erase_flash")

env.AddPreAction("upload", before_upload)