open Utils;

requireCSS("src/CommentsPage.css");

type state = {
  story_with_comments: option(StoryData.story_with_comments)
};

[@bs.deriving {accessors: accessors}]
type action =
  | Loaded(StoryData.story_with_comments);

let component = ReasonReact.reducerComponent("CommentsPage");
let make = (~id, _children) => {

  let renderTitle = (story: StoryData.story_with_comments) => {
    let title = <h2 className="CommentsPage_title"> (textEl(story.title)) </h2>;

    <div> 
      (
        switch story.url {
        | Some(url) => <a href=url className="CommentsPage_titleLink"> title </a>
        | None => title
        }
      )
    </div>
  };

  let renderByline = (story: StoryData.story_with_comments) =>
    <div>
      <span> (intEl(story.score)) </span>
      (textEl(" points"))
      <span>
        <span>
          ({
            let time = story.time;
            let by = story.by;
            textEl({j| submitted $time by $by|j})
          })
        </span>
      </span>
    </div>;

  {
    ...component,

    initialState: () => {
      story_with_comments: None
    },

    reducer: (action, _state) =>
      switch action {
      | Loaded(data) => ReasonReact.Update({
          story_with_comments: Some(data)
        })
      },

    didMount: (self) => {
      StoryData.fetchStoryWithComments(id, self.reduce(loaded));
      ReasonReact.NoUpdate
    },

    render: ({state}) => {
      <div>
        (
          switch state.story_with_comments {
          | Some(story) =>
            <div> (renderTitle(story)) (renderByline(story)) <CommentList story /> </div>
          | None => textEl("loading")
          }
        )
      </div>
    }
  }
};
