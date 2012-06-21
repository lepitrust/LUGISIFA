using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;

namespace Prova_Servizio
{
    public partial class Service_PROVA : ServiceBase
    {
        public Service_PROVA()
        {
            InitializeComponent();
            if (!System.Diagnostics.EventLog.SourceExists("MySource"))
            {
                System.Diagnostics.EventLog.CreateEventSource("MySource", "MyNewLog");
            }
            eventLog1.Source = "MySource";
            eventLog1.Log = "MyNewLog";
        }

        protected override void OnStart(string[] args)
        {
            eventLog1.WriteEntry("In OnStart");
        }

        protected override void OnStop()
        {
            eventLog1.WriteEntry("In OnStop");
        }

        //NB: é possibile ridefinire il comportamento OnContinue, OnPause e OnShutDown allo stesso modo
    }
}
