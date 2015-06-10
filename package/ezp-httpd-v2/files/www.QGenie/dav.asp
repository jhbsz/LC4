<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8" />
<title><% nv_get("model"); %> - File Station Lite</title>


<!-- Always set width and height for html and body if you would like to set width and height for Ajax File Browser control to 100% -->
<style type="text/css">
    html, body {margin: 0px; padding: 0px; width: 100%; height: 100%;}
</style>

<%do_custom_js();%>
<%do_basic_js();%>

</style>

 

<script src='ITHitAJAXFileBrowserLoader.js' type='text/javascript'></script>

 

<script type='text/javascript'>

function InitAjaxFileBrowser() {
    var url = window.location.host;
    var webDavServerPath = "http://" + url + "/fslite";
	var phrases = {
        // File browser phrases.
        AjaxFileBrowser: {

            Commands: {
            UndoCommand: '<%lang("Undo");%> {0}',
                CancelCommand: '<%lang("Cancel");%> {0}',
                Names: {
                    Load: '<%lang("Load");%>',
                    Copy: '<%lang("Copy");%>',
                    CreateFolder: '<%lang("Create Folder");%>',
                    Delete: '<%lang("Delete");%>',
                    LoadFolder: '<%lang("Load Folder");%>',
                    LoadItem: '<%lang("Load Item");%>',
                    LoadTree: '<%lang("Load Tree");%>',
                    Move: '<%lang("Move");%>',
                    Rename: '<%lang("Rename");%>',
                    Search: '<%lang("Search");%>'
                },
                Process: {
                    Load: '<%lang("Loading...");%>',
                    Copy: '<%lang("Copying");%> "{0}"...',
                    CreateFolder: '<%lang("Creating Folder");%> "{0}"...',
                    Delete: '<%lang("Deleting");%> "{0}"...',
                    LoadFolder: '<%lang("Loading Folder");%> "{0}"...',
                    LoadItem: '<%lang("Loading Item");%> "{0}"...',
                    LoadTree: '<%lang("Loading Tree");%> "{0}"...',
                    Move: '<%lang("Moving");%> "{0}"...',
                    Rename: '<%lang("Renaming");%> "{0}"...',
                    Search: '<%lang("Searching");%> "{0}"...'
                }
            },

            Interface: {

                Buttons: {
                    Copy:                '<%lang("Copy");%>',
                    CustomProp:          '<%lang("Custom Properties");%>',
                    Cut:                 '<%lang("Cut");%>',
                    Close:               '<%lang("Close");%>',
                    Delete:              '<%lang("Delete");%>',
                    Details:             '<%lang("Details");%>',
                    Download:            '<%lang("Download");%>',
                    Edit:                '<%lang("Edit");%>',
                    ExtraLargeIcons:     '<%lang("Extra Large Icons");%>',
                    FileManager:         '<%lang("Open OS\'s File Manager.");%>',
                    Folders:             '<%lang("Folders");%>',
                    Info:                '<%lang("Properties");%>',
                    LargeIcons:          '<%lang("Large Icons");%>',
                    Lock:                '<%lang("Lock");%>',
                    Log:                 '<%lang("Log");%>',
                    MediumIcons:         '<%lang("Medium Icons");%>',
                    NewFolder:           '<%lang("New Folder");%>',
                    Paste:               '<%lang("Paste");%>',
                    Redo:                '<%lang("Redo");%>',
                    Refresh:             '<%lang("Refresh");%>',

                    VersionUpdateToThis: '<%lang("Update");%>',
                    VersionRemove:       '<%lang("Remove");%>',
                    VersionDownload:     '<%lang("Download");%>',
                    Versions:            '<%lang("Versions");%>',
                    Views:               '<%lang("Views");%>',
                    Undo:                '<%lang("Undo");%>',
                    Unlock:              '<%lang("Unlock");%>',
                    Up:                  '<%lang("Up");%>',
                    Upload:              '<%lang("Upload");%>',
                    UploadFolder:        '<%lang("Upload Folder");%>'
                },

                Add:                                       '<%lang("Add");%>',
                AddProperty:                               '<%lang("Add Property");%>',
                Address:                                   '<%lang("Address:");%>',
                BrowseFiles:                               '<%lang("Browse...");%>',
                BuyNow:                                    '<%lang("Buy Now");%>',
                Cancel:                                    '<%lang("Cancel");%>',
                CannotCreateDocument:                      '<%lang("Selected document is not created.");%>',
                CannotEditFile:                            '<%lang("Current file cannot be opened for editing.");%>',
                Close:                                     '<%lang("Close");%>',
                CopyPattern:                               '<%lang("Copy");%>',
                Create:                                    '<%lang("Create");%>',
                Created:                                   '<%lang("Created:");%>',
                CrossDomainRequestsIssue:                  '<%lang("Your server may not be configured to allow cross-domain requests.");%><br/><%lang("Please see how to enable cross-domain requests");%> <a target="_blank" href="http://www.webdavsystem.com/ajaxfilebrowser/programming/cross_domain">here</a>.<br/>',
                CrossDomainRequestsIssueIE:                '<%lang("Your Internet Explorer may not be configured to allow cross-domain requests.");%><br/><%lang("Please see how to enable cross-domain requests in Interner Explorer 9 and earlier");%> <a target="_blank" href="http://www.webdavsystem.com/ajaxfilebrowser/programming/cross_domain">here</a>.<br/>',
                CrossDomainRequestsIssueTitle:             '<%lang("Failed to connect to server");%>',
                DateModified:                              '<%lang("Date Modified");%>',
                Delete:                                    '<%lang("Delete");%>',
                DeleteItemsConfirmQuestion:                '<%lang("Are you sure you want to delete selected item(s)?");%>',
                DeletePropertiesConfirmQuestion:           '<%lang("Are you sure want to delete selected property(s)?");%>',
                DeleteVersionConfirmQuestion:              '<%lang("Are you sure want to delete selected version?");%>',
                Details:                                   '<%lang("Details");%>',
                DropFilesIEUnsupported:                    '<%lang("Files drag and drop from file system is supported only in Internet Explorer 10, Chrome 4, FireFox 3.6, Safari 4 and later versions.");%>',
                Edit:                                      '<%lang("Edit");%>',
                EditProperty:                              '<%lang("Edit Рroperty");%>',
                Error:                                     '<%lang("Error:");%>',
                ErrorDetails:                              '<%lang("Error Details");%>',
                FailedToOpenWinExplorer:                   '<%lang("Failed to open WebDAV folder in Windows Explorer.");%>',
                File:                                      '<%lang("File");%>',
                FileExistsOverwriteConfirmQuestion:        '<%lang("File");%> "{0}" <%lang("already exists. Do you want to overwrite it?");%>',
                FileFolderType:                            '<%lang("Folder");%>',
                FileNameSpecialCharactersRestriction:      '<%lang("The name cannot contain any of the following characters:");%>\n\t\t{0}',
                FilePrecentInUploadList:                   '<%lang("Selected file is already present in upload list.");%>',
                FilesPresentInUploadList:                  '<%lang("The following file(s) is present in the upload list:");%>\n\n{0}',
                FilesResumeNotInListOrChanged:             '<%lang("The following file(s) was modified:");%>\n{0}\n\n'+
                    '<%lang("The following file(s) was not among your selection for resume in the upload list:");%>\n{1}\n\n<%lang("The upload will start from the begging of each file.");%>',
                FileUploadFailed:                          '<%lang("Failed to upload the file.");%>',
                FillOutFileName:                           '<%lang("Specify the file name.");%>',
                FillOutFolderName:                         '<%lang("Specify the folder name.");%>',
                FirefoxLargeFilesToUpload:                 '<%lang("In Firefox files over 100 Mb cannot be uploaded together with other files or via drag and drop. The following files are too large:");%>\n\n<b>{0}</b>\n\n'+
                                                               '<%lang("To upload this file(s) you must select them one by one using \'Browse...\' button or use other browser.");%>',
                FirefoxFolderDropNotSupported:             '<%lang("Folders drag and drop is available only in Google Chrome 21 and later versions.");%>',
                ItemAlreadyExists:                         '<%lang("Item already exists.");%>',
                FolderAlreadyExists:                       '<%lang("Folder already exists.");%>',
                FolderName:                                '<%lang("Folder name");%>',
                FolderUploadNotAvailedInYourBrowser:       '<%lang("Folders upload is available only in Google Chrome 21 and later versions.");%>',
                FollowingErrorsOccurred:                   '<%lang("The Following Errors Has Occurred");%>',
                FreeSpace:                                 '<%lang("Free space:");%>',
                GetPropertiesError:                        '<%lang("Properties are not selected.");%>',
                GetVersionsError:                          '<%lang("Versions are not selected.");%>',
                HttpNotAvailableError:                     '<%lang("Server is not available.");%>',
                Item:                                      '<%lang("Item");%>',
                ItemType:                                  '<%lang("Item type:");%>',
                LocalResource:                             '<%lang("Local Resource");%>',
                Location:                                  '<%lang("Location:");%>', // Item URI.
                Log:                                       '<%lang("Log");%>',
                ManageProperties:                          '<%lang("Manage Properties");%>',
                Message:                                   '<%lang("Message");%>',
                Modified:                                  '<%lang("Modified:");%>',
                MoveFolderToSubfolderRestriction:          '<%lang("The destination folder is a subfolder of the source folder.");%>',
                MoveFileToSameDirectoryRestriction:        '<%lang("The source and destination file names are the same.");%>',
                Name:                                      '<%lang("Name");%>',
                Namespace:                                 '<%lang("Namespace");%>',
                NewFolder:                                 '<%lang("New Folder");%>',
                NewFolderPattern:                          '<%lang("New Folder");%>',
                NewName:                                   '<%lang("New name");%>',
                No:                                        '<%lang("No");%>',
                NonExistentLocation:                       '<%lang("Location does not exist.");%>',
                NotFoundLocation:                          '<%lang("Location");%> "{0}" <%lang("not found.");%>',
                NoToAll:                                   '<%lang("No to All");%>',
                OK:                                        '<%lang("OK");%>',
                OperationCannotBePerformed:                '<%lang("Operation Cannot Be Performed:");%>',
                OverwriteConfirm:                          '<%lang("Overwrite Confirm");%>',
                Properties:                                '<%lang("Properties");%>',
                ProtectedForRunningFileExtension:          '<%lang("File with extension");%> "{0}" <%lang("protected for running.");%>',
                PropertyNameRestriction:                   '<%lang("Property name and namespace can not be empty and must begin with a letter, may be followed by letters, digits, hyphens, underscores and periods.");%>',
                Rename:                                    '<%lang("Rename");%>',
                RequestError:                              '<%lang("Error");%>',
                Safari51WindowsMultiUploadNotSupported:    '<%lang("Safari 5.1.x does not support multiselection via \'Browse\' button. Please use drag and drop or select a single file only or use other web browser.");%>',
                Search:                                    '<%lang("Search:");%>',
                Size:                                      '<%lang("Size:");%>',
                Source:                                    '<%lang("Source");%>',
                SpecifyItemName:                           '<%lang("Specify the item name");%>',
                TargetFolder:                              '<%lang("Target Folder");%>',
                TotalSpace:                                '<%lang("Total space:");%>',
                Type:                                      '<%lang("Type");%>',
                TrialPeriodExpired:                        '<%lang("The trial period has expired.");%>',
                UpdateFile:                                '<%lang("Update File");%>',
                Upload:                                    '<%lang("Upload");%>',
                UploadCaption:                             '<%lang("Upload");%>',
                UploadCaptionForDnDFilesSupportedBrowsers: '<%lang("To upload drag and drop files here or into folder structure.");%>',
                UploadFile:                                '<%lang("Upload Files");%>',
                UploadInProgressWarning:                   '<%lang("One or more files are being uploaded. If you close this page upload will be paused.");%>',
                UsedPercent:                               '<%lang("Used:");%> {0}%',
                UsedSpace:                                 '<%lang("Used space:");%>',
                Value:                                     '<%lang("Value");%>',
                Versions:                                  '<%lang("Versions");%>',
                VersionUpdateToThisTooltip:                '<%lang("Update to this version");%>',
                Yes:                                       '<%lang("Yes");%>',
                YesToAll:                                  '<%lang("Yes to All");%>',
                WarningOldVersion:                         '<%lang("Ajax File Browser has detected that your IT Hit WebDAV server version is");%> {0}. <%lang("For correct versions support it is recommended to upgrade to v3.5.x or later version. To avoid this message set SuppressServerWarning=true when creating Ajax File Browser.");%>',
                WebFoldersUpdate:                          '<%lang("Please install");%> <a target="_blank" href="{0}">Software Update for Web Folders</a>.'
            },

            FileSizes: {
                Bytes: '<%lang("Bytes");%>',
                B:     '<%lang("B");%>',
                GB:    '<%lang("GB");%>',
                KB:    '<%lang("KB");%>',
                MB:    '<%lang("MB");%>'
            },

            Menu: {
                ArrangeIconsBy:           '<%lang("Arrange Icons By");%>',
                CancelUpload:             '<%lang("Cancel");%>',
                Copy:                     '<%lang("Copy");%>',
                CustomProperties:         '<%lang("Custom Properties...");%>',
                Cut:                      '<%lang("Cut");%>',
                Delete:                   '<%lang("Delete");%>',
                Details:                  '<%lang("Details");%>',
                EditDocument:             '<%lang("Edit Document");%>',
                ExtraLargeIcons:          '<%lang("Extra Large Icons");%>',
                LargeIcons:               '<%lang("Large Icons");%>',
                Lock:                     '<%lang("Lock...");%>',
                Name:                     '<%lang("Name");%>',
                NewDocument:              '<%lang("New Document");%>',
                NewFolder:                '<%lang("New Folder...");%>',
                NewWord:                  '<%lang("Microsoft Office Word Document");%>',
                NewExcel:                 '<%lang("Microsoft Office Excel Worksheet");%>',
                NewPowerPoint:            '<%lang("Microsoft Office PowerPoint Presentation");%>',
                MediumIcons:              '<%lang("Medium Icons");%>',
                Modified:                 '<%lang("Modified");%>',
                OpenContainingFolderInFM: '<%lang("Open Containing Folder");%>',
                OpenDownload:             '<%lang("Download");%>',
                OpenFolderInFM:           '<%lang("Open Folder in File Manager");%>',
                OpenLocation:             '<%lang("Open Item Location");%>',
                Paste:                    '<%lang("Paste");%>',
                PauseUpload:              '<%lang("Pause");%>',
                Properties:               '<%lang("Properties...");%>',
                Refresh:                  '<%lang("Refresh");%>',
                Rename:                   '<%lang("Rename...");%>',
                RestartUpload:            '<%lang("Restart Upload");%>',
                ResumeUpload:             '<%lang("Resume");%>',
                Size:                     '<%lang("Size");%>',
                Type:                     '<%lang("Type");%>',
                Unlock:                   '<%lang("Unlock...");%>',
                UpdateFile:               '<%lang("Update File...");%>',
                UploadFile:               '<%lang("Upload Files...");%>',
                UploadFolder:             '<%lang("Upload Folder...");%>',
                Versions:                 '<%lang("Versions...");%>',
                View:                     '<%lang("View");%>',
                ViewError:                '<%lang("View Error...");%>'
            },

            Grid: {
                DateModified:   '<%lang("Date Modified");%>',
                FileTypePrefix: '<%lang("File");%> ',
                Name:           '<%lang("Name");%>',
                Size:           '<%lang("Size");%>',
                Type:           '<%lang("Type");%>',
                Folder:         '<%lang("Folder");%>'
            },

            Property: {
                Name:      '<%lang("Name");%>',
                Namespace: '<%lang("Namespace");%>',
                Value:     '<%lang("Value");%>'
            },

            Version: {
                Name:         '<%lang("Version Name");%>',
                CreationDate: '<%lang("Creation Date");%>',
                Size:         '<%lang("Size");%>',
                Comments:     '<%lang("Comments");%>',
                SavedBy:      '<%lang("Saved By");%>'
            },

            Upload: {
                Destination:     '<%lang("Destination");%>',
                Elapsed:         '<%lang("Elapsed");%>',
                Error:           '<%lang("Error");%>',
                FileSize:        '<%lang("File Size");%>',
                Left:            '<%lang("Left");%>',
                Paused:          '<%lang("Paused");%>',
                Progress:        '<%lang("Progress");%>',
                ProgressPercent: '<%lang("Status");%>',
                Queued:          '<%lang("Queued");%>',
                RetryIn:         '<%lang("Retry in");%> {0}<%lang("s");%>',
                Source:          '<%lang("File");%>',
                Speed:           '<%lang("Speed");%>',
                Uploaded:        '<%lang("Uploaded");%>'
            },

            StatusDescriptions: {},

            Exceptions: {
                TrySearchByResource:                           '<%lang("You try to search by resource. Only folders support search.");%>',
                BytesUploadedIsMoreThanTotalFileContentLength: '<%lang("Bytes uploaded cannot be more than total file content length.");%>',
                DestinationLocationNotPassed:                  '<%lang("Destination URL is not passed.");%>',
                EmptyElementParameter:                         '<%lang("Element parameter is empty.");%>',
                EmptyUrlProvided:                              '<%lang("Please specify WebDAV server url.");%>',
                FolderInstanceExpected:                        '<%lang("ITHit.WebDAV.Client.AjaxFileBrowser.Folder instance object expected as input parameter.");%>',
                FunctionalityNotSupported:                     '<%lang("Functionality {0} is not supported by your browser.");%>',
                HierarchyItemInstanceExpected:                 '<%lang("ITHit.WebDAV.Client.AjaxFileBrowser.HierarchyItem instance object expected as input parameter.");%>',
                InterfaceLabelIsNotPassed:                     '<%lang("Interface label is not passed.");%>',
                KeyCodeNotSpecified:                           '<%lang("Key code not specified. Passed:");%> {0}',
                NonExistentLocation:                           '<%lang("Location "{0}" does not exist.");%>',
                ResourceInstanceExpected:                      '<%lang("ITHit.WebDAV.Client.AjaxFileBrowser.Resource instance object expected as input parameter.");%>',
                SessionNotStarted:                             '<%lang("Session is not started. Possible reason: ITHit WebDAV AJAX Library is not loaded.");%>',
                UnexpectedUploadItemState:                     '<%lang("Unexpected upload item state:");%> {0}',
                UploadProgressInfoInstanceExpected:            '<%lang("ITHit.WebDAV.Client.AjaxFileBrowser.InternalUploadProgressInfo instance expected.");%>',
                WrongBaseUrl:                                  '<%lang("Incorrect URL provided or URl is empty.");%>',
                WrongContentLengthType:                        '<%lang("File content length is expected to be an integer.");%>',
                WrongCountAsMaximumUploadTasks:                '<%lang("Expected integer value above zero as count of maximum upload tasks at the same time. Passed:");%> {0}',
                WrongCountInputParameters:                     '<%lang("Wrong count of imput parameters passed.");%>',
                WrongElementIdOrObject:                        '<%lang("Expected element as string id or object.");%>',
                WrongInterfaceHandler:                         '<%lang("Wrong interface handler indentifier. Passed:");%> {0}',
                WrongMenuItemType:                             '<%lang("Passed wrong menu item type. Passed:");%> {0}',
                WrongParametrType:                             '<%lang("Wrong parameter type.");%>',
                WrongTemplateIndex:                            '<%lang("Passed wrong template index.");%>',
                WrongUploadedBytesType:                        '<%lang("Count of uploaded bytes expected to be a integer.");%>',
                WrongUrl:                                      '<%lang("Passed wrong URL:");%> "{0}".',
                WrongViewModeIndex:                            '<%lang("Passed wrong view mode index:");%> "{0}".'
            },

            ContextHelp: {
                Commands: {
                    CancelUpload:       '<%lang("Cancel upload of the selected file.");%>',
                    Copy:               '<%lang("Copy the selected items to the clipboard. To put them in the new location, use the Paste command.");%>',
                    CustomProp:         '<%lang("Manage custom properties associated with the selected item.");%>',
                    Cut:                '<%lang("Cut the selected items to buffer.");%>',
                    Delete:             '<%lang("Deletes the selected items.");%>',
                    Download:           '<%lang("Download the selected file.");%>',
                    Edit:               '<%lang("Edit the selected file. Open document directly from server for editing with associated application.");%>',
                    FileManager:        '<%lang("Open this folder in Operating System\'s file manager.");%>',
                    Folders:            '<%lang("Show or hide navigation tree.");%>',
                    Info:               '<%lang("Show information about the selected item.");%>',
                    Lock:               '<%lang("Lock the selected file. The item will be protected from modification. Note that you would not be able to edit the file as well.");%>',
                    Log:                '<%lang("Show log. Send this log to administrator if you experience any issues, togather with a description and screenshots.");%>',
                    NewDocument:        '<%lang("Create a new Microsoft Office document.");%>',
                    NewExcel:           '<%lang("Create a new Microsoft Office Excel document.");%>',
                    NewFolder:          '<%lang("Create a new folder");%>',
                    NewPowerPoint:      '<%lang("Creates a new Microsoft Office PowerPoint document.");%>',
                    NewWord:            '<%lang("Creates new Microsoft Office Word document.");%>',
                    OpenLocation:       '<%lang("Navigate to folder that contains this file.");%>',
                    Paste:              '<%lang("Paste the content of the buffer to the current location.");%>',
                    PauseUpload:        '<%lang("Pause upload of the selected file(s).");%>',
                    RedoButton:         '<%lang("Redo undone operation.");%>',
                    Refresh:            '<%lang("Refresh content of this folder.");%>',
                    Rename:             '<%lang("Rename the selected item.");%>',
                    RestartUpload:      '<%lang("Restart file upload.");%>',
                    ResumeUpload:       '<%lang("Resume upload of the selected file(s).");%>',
                    Undo:               '<%lang("Undo previous operation.");%>',
                    Unlock:             '<%lang("Unlock item.");%>',
                    Up:                 '<%lang("Navigate one level up in the folder structure.");%>',
                    UpdateFile:         '<%lang("Update file content. Your file will be overwritten with a selected file.");%>',
                    Upload:             '<%lang("Upload files to selected folder.");%>',
                    UploadFolder:       '<%lang("Upload local folder with all contained files to selected folder.");%>',
                    Versions:           '<%lang("Manage file versions. Download previous file versions, delete and rollback to previous versions.");%>',
                    Views:              '<%lang("Displays items using thumbnails or details view.");%>',
                    ViewsDetails:       '<%lang("Displays items using details view.");%>',
                    ViewsMedium:        '<%lang("Displays items using medium view.");%>',
                    ViewsLarge:         '<%lang("Displays items using large view.");%>',
                    ViewsExtraLarge:    '<%lang("Displays items using extra large view.");%>',
                    ViewError:          '<%lang("Show upload errors.");%>'
                }
            }
        },

        Exceptions: {
            ObjectExpected: '<%lang("Object or null expected.");%>',
            UndefinedEventHandler: '<%lang("Event handler");%> {0}::{1} <%lang("is undefined.");%>'
        }
	}

 
    var settings = {
        BasePath: '/',
        Id: 'AjaxFileBrowserContainer',
        Url: webDavServerPath,
        Style: 'height: 100%; width: 100%;',

        MsOfficeTemplatesPath: 'http://' + url + '/templates/',
        SelectedFolder: webDavServerPath,
        ThemeName: 'windows_8',
        IconsSize: 16,
        Platform: 'auto',
        SearchAutocompleteEnable: true,
        SearchDynamicEnable: true,

        PluginsPath: "http://" + url + "/plugins/",

        Panels: {
            Folders: { Show: true },
            FilesView: { Show: true },
            Toolbar: {
                Show: true,
                Items: [
                    {
                        Id: 'UploadFolderButton',
                        show: false
                    }
                ]
            },
            AddressBar: { Show: false },
            UploadPanel: { Show: false },
            UploadProgressPanel: { Show: true }
        },
		Phrases: phrases
    };

    var ajaxFileBrowserLoader = new ITHit.WebDAV.Client.AjaxFileBrowserLoader(settings);
    ajaxFileBrowserLoader.LoadAsync();
    to_update_time();
}
</script>

 

</head>
<body onload="InitAjaxFileBrowser()">
<div id="AjaxFileBrowserContainer" class="ih_style" style="width: 100%; height: 100%"></div>
</body>
</html>
