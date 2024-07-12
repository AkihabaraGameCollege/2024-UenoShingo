using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

// ステージ画面の進行を制御します。
public class StageScene : MonoBehaviour
{
    // このステージをクリアーしたときに読み込むシーンを指定します。
    [SerializeField]
    private string nextStage = "GameClear";

    // プレイヤーを指定します。
    [SerializeField]
    private Player player = null;

    // ステージ イントロ表示用のUIを指定します。
    [SerializeField]
    private StageIntroUI stageIntroUI = null;
    // ゲームオーバー表示用のUIを指定します。
    [SerializeField]
    private GameOverUI gameOverUI = null;
    // ステージクリアー表示用のUIを指定します。
    [SerializeField]
    private StageClearUI stageClearUI = null;
    // 楽曲再生用の AudioSource を指定します。
    [SerializeField]
    private AudioSource musicAudio = null;

    // コンポーネントを事前に参照しておく変数
    Animator animator;
    // AnimatorパラメーターID
    static readonly int outroId = Animator.StringToHash("Outro");

    // ステージ画面内の進行状態を表します。
    enum SceneState
    {
        // ステージ開始演出中
        Intro,
        // ステージプレイ中
        Play,
        // ゲームオーバーが確定していて演出中
        GameOver,
        // ステージクリアーが確定していて演出中
        StageClear,
    }
    SceneState gameState = SceneState.Intro;

    // 自分自身のインスタンスを取得します。
    public static StageScene Instance { get; private set; } = null;

    // Start()よりも先に実行されます。
    private void Awake()
    {
        // 自分自身をインスタンスとして保存
        Instance = this;
    }

    // Start is called before the first frame update
    void Start()
    {
        // コンポーネントを事前に取得
        animator = GetComponent<Animator>();

        // UnityEvent を追加
        stageIntroUI.onPlayGame.AddListener(PlayGame);
        gameOverUI.onRetryButtonClick.AddListener(Retry);
        gameOverUI.onExitButtonClick.AddListener(Exit);
        stageClearUI.onNextButtonClick.AddListener(LoadNextStage);

        gameState = SceneState.Intro;
        player.Sleep();
    }

    // ステージを開始します。
    public void PlayGame()
    {
        gameState = SceneState.Play;
        player.WakeUp();
    }

    // このステージをゲームオーバーとします。
    public void GameOver()
    {
        // ステージプレイ中のみ
        if (gameState == SceneState.Play)
        {
            gameState = SceneState.GameOver;
            musicAudio.Stop();
            // ゲームオーバーUIを表示
            gameOverUI.Show();
        }
    }

    // このステージを再読み込みします。
    public void Retry()
    {
        StartCoroutine(OnLoadScene(SceneManager.GetActiveScene().name));
    }

    // このステージを抜けてタイトル画面を読み込みます。
    public void Exit()
    {
        StartCoroutine(OnLoadScene("Title"));
    }

    // このステージをステージクリアーとします。
    public void StageClear()
    {
        // ステージプレイ中のみ
        if (gameState == SceneState.Play)
        {
            gameState = SceneState.StageClear;
            musicAudio.Stop();
            // ステージクリアーUIを表示
            stageClearUI.Show();
        }
    }

    // 次のステージを読み込みます。
    public void LoadNextStage()
    {
        StartCoroutine(OnLoadScene(nextStage));
    }

    // 指定したシーンを読み込みます。
    IEnumerator OnLoadScene(string sceneName)
    {
        // アウトロ アニメーションへ切り替える
        animator.SetTrigger(outroId);
        // アニメーションが終了するまで1秒待機
        yield return new WaitForSeconds(1);
        // シーンをロードする
        SceneManager.LoadScene(sceneName);
    }
}
