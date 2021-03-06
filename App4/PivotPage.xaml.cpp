﻿//
// PivotPage.xaml.cpp
// Implémentation de la classe PivotPage.
//

#include "pch.h"
#include "PivotPage.xaml.h"
#include "ItemPage.xaml.h"
#include <string>
#include <iostream>
#include <thread>
#include <cstdio>
#include <ctime>
#include <sstream>
#include <mutex>

using namespace App4;
using namespace App4::Common;
using namespace App4::Data;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::ApplicationModel::Resources;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace std;
using namespace Windows::Devices::Geolocation;

// Pour plus d'informations sur le modèle Application Pivot, consultez la page http://go.microsoft.com/fwlink/?LinkID=391641
static std::thread threadChrono;
static std::thread threadGPS;
static void taskChrono(int* s);

mutex mutexChrono;

//static std::thread threadApp;
Windows::Foundation::IAsyncOperation<Windows::Devices::Geolocation::Geoposition^>^ m_getOperation;
std::clock_t start;
double duration;

PivotPage::PivotPage()
{
	InitializeComponent();

	NavigationCacheMode = Navigation::NavigationCacheMode::Required;

	_resourceLoader = ResourceLoader::GetForCurrentView(L"Resources");

	auto navigationHelper = ref new Common::NavigationHelper(this);
	navigationHelper->LoadState += ref new LoadStateEventHandler(this, &PivotPage::NavigationHelper_LoadState);
	navigationHelper->SaveState += ref new SaveStateEventHandler(this, &PivotPage::NavigationHelper_SaveState);

	SetValue(_defaultViewModelProperty, ref new Platform::Collections::Map<String^, Object^>(std::less<String^>()));
	SetValue(_navigationHelperProperty, navigationHelper);

	int time;
	// Constructs the new thread and runs it. Does not block execution.
	threadChrono = thread(taskChrono, &time);
	threadGPS = thread(App4::PivotPage::taskGPS);

	
}

DependencyProperty^ PivotPage::_navigationHelperProperty = nullptr;
DependencyProperty^ PivotPage::_defaultViewModelProperty = nullptr;

void PivotPage::RegisterDependencyProperties()
{
	if (_navigationHelperProperty == nullptr)
	{
		_navigationHelperProperty = DependencyProperty::Register("NavigationHelper",
			TypeName(Common::NavigationHelper::typeid), TypeName(PivotPage::typeid), nullptr);
	}

	if (_defaultViewModelProperty == nullptr)
	{
		_defaultViewModelProperty =
			DependencyProperty::Register("DefaultViewModel",
			TypeName(IObservableMap<String^, Object^>::typeid), TypeName(PivotPage::typeid), nullptr);
	}
}


IObservableMap<String^, Object^>^ PivotPage::DefaultViewModel::get()
{
	return safe_cast<IObservableMap<String^, Object^>^>(GetValue(_defaultViewModelProperty));
}

NavigationHelper^ PivotPage::NavigationHelper::get()
{
	return safe_cast<Common::NavigationHelper^>(GetValue(_navigationHelperProperty));
}

void PivotPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedTo(e);
}

void PivotPage::OnNavigatedFrom(NavigationEventArgs^ e)
{
	NavigationHelper->OnNavigatedFrom(e);
}

/// <summary>
/// Remplit la page à l'aide du contenu passé lors de la navigation. Tout état enregistré est également
/// fourni lorsqu'une page est recréée à partir d'une session antérieure.
/// </summary>
/// <param name="sender">
/// La source de l'événement, en général <see cref="NavigationHelper"/>.
/// </param>
/// <param name="e">Données d'événement qui fournissent le paramètre de navigation transmis à
/// <see cref="Frame->Navigate(Type, Object)"/> lors de la requête initiale de cette page et
/// un dictionnaire d'état conservé par cette page durant une session
/// antérieure. L'état n'aura pas la valeur Null lors de la première visite de la page.</param>
void PivotPage::NavigationHelper_LoadState(Object^ sender, LoadStateEventArgs^ e)
{
	(void) sender;	// Paramètre non utilisé
	(void) e;		// Paramètre non utilisé

	SampleDataSource::GetGroup(L"Group-1").then([this](SampleDataGroup^ sampleDataGroup)
	{
		DefaultViewModel->Insert(GetFirstGroupName(), sampleDataGroup);
	}, task_continuation_context::use_current());
}

/// <summary>
/// Conserve l'état associé à cette page en cas de suspension de l'application ou de
/// suppression de la page du cache de navigation. Les valeurs doivent être conforme aux
/// exigences en matière de sérialisation de <see cref="SuspensionManager.SessionState"/>.
/// </summary>
/// <param name="sender">
/// La source de l'événement, en général <see cref="NavigationHelper"/>.
/// </param>
/// <param name="e">
/// Données d'événement qui fournissent un dictionnaire vide à remplir avec un état sérialisable.
/// </param>
void PivotPage::NavigationHelper_SaveState(Object^ sender, SaveStateEventArgs^ e)
{
	// TODO: enregistrer l'état unique de la page ici.
}

/// <summary>
/// Charge le contenu pour le second élément Pivot lorsqu'il devient visible.
/// </summary>
void PivotPage::SecondPivot_Loaded(Object^ sender, RoutedEventArgs ^e)
{
	(void) sender;	// Paramètre non utilisé
	(void) e;		// Paramètre non utilisé

	// TODO: créer un modèle de données approprié pour le domaine posant problème afin de remplacer les exemples de données.
	SampleDataSource::GetGroup(L"Group-2").then([this](SampleDataGroup^ sampleDataGroup)
	{
		DefaultViewModel->Insert(GetSecondGroupName(), sampleDataGroup);
	}, task_continuation_context::use_current());
}

/// <summary>
/// Ajoute un élément à la liste en cas de clic sur le bouton de la barre de l'application.
/// </summary>
void PivotPage::AddAppBarButton_Click(Object^ sender, RoutedEventArgs ^e)
{
	PivotPage::GetOneShotLocation(5, 10,  10);
	OutputDebugString(L"AddAppBarButton_Click!");
}

/// <summary>
/// Appelé lorsqu'un utilisateur clique sur un élément appartenant à une section.
/// </summary>
void PivotPage::ItemView_ItemClick(Object^ sender, ItemClickEventArgs ^e)
{
	auto itemId = safe_cast<SampleDataItem^>(e->ClickedItem)->UniqueId;
	if (!Frame->Navigate(ItemPage::typeid, itemId))
	{
		throw ref new FailureException(_resourceLoader->GetString(L"NavigationFailedExceptionMessage"));
	}
}


static void taskChrono(int *s)
{
	for (;;) {
		start = std::clock();
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		std::cout << "printf: " << duration << '\n';

		cout << "taskChrono says: ";
		wait(2000);
	}
}

void PivotPage::taskGPS()
{

	for (;;) {
		cout << "taskGPS says: ";
		wait(2000);
	}
}

void PivotPage::GetOneShotLocation(int accuracyInMeters, int timeoutSeconds, int maxAgeSeconds) {
	Geolocator^ geolocator = ref new Geolocator();

	geolocator->DesiredAccuracyInMeters = (Platform::IBox<UINT>^)(PropertyValue::CreateUInt32(accuracyInMeters));

	m_getOperation = nullptr;

	if (timeoutSeconds > 0 || maxAgeSeconds > 0)
	{
		TimeSpan maxAge;
		maxAge.Duration = maxAgeSeconds * 10000;

		TimeSpan timeout;
		timeout.Duration = timeoutSeconds * 10000;
		m_getOperation = geolocator->GetGeopositionAsync(maxAge, timeout);
	}
	else
	{
		// Use the API with defaults
		m_getOperation = geolocator->GetGeopositionAsync();
	}

	// Start location acquisition.
	// Setting the completion callback implicitly starts acquisition.
	m_getOperation->Completed = ref new AsyncOperationCompletedHandler<Geoposition^>(
		[=](IAsyncOperation<Geoposition^>^ asyncOperation, AsyncStatus status) mutable
	{
		if (status != AsyncStatus::Error)
		{
			Geoposition^ geoposition = asyncOperation->GetResults();

			// use the location information
			double latitude = geoposition->Coordinate->Latitude;
			double longitude = geoposition->Coordinate->Longitude;
			double accuracy = geoposition->Coordinate->Accuracy;

			stringstream ss;
			ss << "Latitude " << latitude << "Longitude" << longitude;
			string s = ss.str();
			wstring stemp = std::wstring(s.begin(), s.end());
			OutputDebugString(stemp.c_str());
		}
		else
		{
			OutputDebugString(L"Button was clicked!");
			if (asyncOperation->ErrorCode.Value == E_ABORT)
			{
				// The user has disable location in the phone settings
			}
			else
			{
				// There was another error
			}
		}
	});

	
}


void App4::PivotPage::StartAppBarButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (!mutexChrono.try_lock()) {
		mutexChrono.lock();
	}
}
