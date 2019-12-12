
#pragma once

namespace GUI
{
	class DLL_EXPORT FileDialog
	{
		public:
			//
			using CtrlValues_t  = std::tuple<std::string, std::string, std::vector<std::tuple<std::string, std::string>>>;
			using BuildValues_t = std::tuple<bool, std::string, std::string, std::string, std::string>;
			//
			virtual ~FileDialog();
			//
			bool        Open(HWND = nullptr);
			bool        Save(HWND = nullptr);
			bool        Folder(HWND = nullptr);
			void        SetCtrlValue(CtrlValues_t const&);
			void        SetCtrlValue(HWND, CtrlValues_t const&);
			std::string GetCtrlString();

		private:
			//
			HWND          m_phwnd = nullptr;
			std::string   m_path;
            CtrlValues_t  m_options;
            //
			BuildValues_t BuildOptions();
			bool          MakeDialog(HWND, OPENFILENAMEA*, DWORD, std::vector<char>&);
	};
};

/*
	// Example:

    GUI::FileDialog fdlg;
    fdlg.SetCtrlValue(
            {
                ".\\",                             // default path
                "default.file.name",               // default save name
                {                                  // filters:
                    { "PNG files", "png" },
                    { "BMP files", "bmp" },
                    { "JPG files", "jpg" },
                    { "CBP files", "cbp" }
                }
            }
        );
    bool b = fdlg.Open(hwnd);
    if (b)
	{
		std::string s = fdlg.GetCtrlString();
		MessageBoxA(hwnd, s.c_str(), "+++", 0);
	}

	// setup use Folder()

    fdlg.SetCtrlValue(
            {
                "c:\\mydir\\otherdir",                             // default startup directory path
                "dialog title",                                    // default dialog title
                {
                }
            }
        );


*/
