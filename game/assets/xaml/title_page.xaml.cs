using System.Windows.Controls;
using System.Windows.Input;
using WpfTools;

namespace game
{
    public class title_page_view_model : PropertyNotifier
    {
        public ICommand play_command { get; } = new DelegateCommand(() => { });
    };

    public partial class title_page : UserControl
    {
        public title_page_view_model view_model { get; } = new();

        public title_page()
        {
            this.InitializeComponent();
        }
    }
}
