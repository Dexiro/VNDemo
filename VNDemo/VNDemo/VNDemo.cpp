// VNDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include <iostream>
#include <vector>
#include <map>

using namespace std;


namespace vn
{
	 typedef int EventIndex;
	 typedef int MomentIndex;
	 typedef int SceneIndex;



	 // Events are the lowest level "thing" that the story is composed of (basically just a function wrapper)
	 // Includes things like;
	 //   - Show Dialogue
	 //   - Play Animation
	 //   - Change Background Image
	 //   - Set Character Image

	 struct IEvent { virtual void Run() = 0; };

	 struct DialogueEvent : public IEvent 
	 {
		  string text;

		  DialogueEvent(string text) : text(text) {};
		  void Run() { cout << text.c_str() << std::endl; }
	 };



	 // A Moment is a sequence of Events that happen simultaneously or without Player Input 
	 //  Example:
	 //		E1 - Set Background Image
	 //		E2 - Set Character Image
	 //		E3 - Play "Fade In" Animation
	 //		E4 - Show Character A Dialogue

	 struct StoryMoment
	 {
		  vector<EventIndex> events;
		  void AddEvent(int i) { events.push_back(i); }
	 };



	 // A Scene is a sequence of Moments, that each require Player Input to advance.
	 //  Example:
	 //		M1 - Show Character A Dialogue (press A to continue)
	 //		M2 - Play Animation, Show Character B Dialogue (prompt player for decision)

	 struct StoryScene
	 {
		  vector<MomentIndex> moments;
		  void AddMoment(int i) { moments.push_back(i); }
	 };




	 struct StoryChapter
	 {
		  vector<DialogueEvent> dialogue_index;
		  vector<IEvent*> event_index;

		  vector<StoryMoment> moment_index;
		  vector<StoryScene> scene_index;

		  int initial_scene = 0;

		  map<string, StoryScene> key_scenes;


	 public:
		  
		  int AddDialogue(string text)
		  {
				event_index.push_back(new DialogueEvent(text));
				return event_index.size()-1;
		  }

		  int AddMoment(StoryMoment moment) { moment_index.push_back(moment); return moment_index.size() - 1; }
		  int AddScene(StoryScene scene) { scene_index.push_back(scene); return scene_index.size() - 1; }
	 };
}



vn::StoryChapter CreateTestChapter()
{
	 using namespace vn;

	 StoryChapter chapter;

	 StoryMoment m1;
	 m1.AddEvent(chapter.AddDialogue("Character A: Stop what you're doing!"));

	 StoryMoment m2;
	 m2.AddEvent(chapter.AddDialogue("Character B: Try and stop me, loser"));

	 StoryMoment m3;
	 m3.AddEvent(chapter.AddDialogue("Character A: Fine, You asked for it"));
	 m3.AddEvent(chapter.AddDialogue("- Character A throws sword -"));

	 StoryMoment m4;
	 m4.AddEvent(chapter.AddDialogue("Character B: Argh"));

	 StoryScene scene_one;
	 scene_one.AddMoment(chapter.AddMoment(m1));
	 scene_one.AddMoment(chapter.AddMoment(m2));
	 scene_one.AddMoment(chapter.AddMoment(m3));
	 scene_one.AddMoment(chapter.AddMoment(m4));

	 chapter.AddScene(scene_one);

	 return chapter;
}


void RunChapter(vn::StoryChapter chapter, int initial_scene)
{
	 int iscene = initial_scene;

	 while (iscene != -1)
	 {
		  auto& scene = chapter.scene_index[iscene];
		  for (auto& imoment : scene.moments)
		  {
				auto& moment = chapter.moment_index[imoment];
				for (auto& ievent : moment.events)
				{
					 chapter.event_index[ievent]->Run();
				}
				std::cin.get();
		  }
	 }
}


int main() // Entry Point
{
    std::cout << "Hello World!\n"; 

	 auto test_chapter = CreateTestChapter();

	 RunChapter(test_chapter, 0);
}
