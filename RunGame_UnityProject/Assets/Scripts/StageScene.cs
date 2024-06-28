using System.Collections;
using UnityEngine;
using UnityEngine.SceneManagement;

// ステージ画面の進行を制御します。
public class StageScene : MonoBehaviour
{
    // このステージをクリアーしたときに読み込むシーンを指定します。
    [SerializeField]
    private string nextStage = "GameClear";

    // ステージ イントロ表示用のUIを指定します。
    [SerializeField]
    private StageIntroUI stageIntroUI = null;
    // ゲームオーバー表示用のUIを指定します。
    [SerializeField]
    private GameObject gameOverUI = null;
    // ステージクリアー表示用のUIを指定します。
    [SerializeField]
    private StageClearUI stageClearUI = null;

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

        // UIを非表示
        gameOverUI.SetActive(false);
        // UnityEvent を追加
        stageIntroUI.onPlayGame.AddListener(PlayGame);
        stageClearUI.onNextButtonClick.AddListener(LoadNextStage);

        gameState = SceneState.Intro;
    }

    // ステージを開始します。
    public void PlayGame()
    {
        gameState = SceneState.Play;
    }

    // このステージをゲームオーバーとします。
    public void GameOver()
    {
        // ステージプレイ中のみ
        if (gameState == SceneState.Play)
        {
            gameState = SceneState.GameOver;
            StartCoroutine(OnGameOver());
        }
    }

    // ゲームオーバー演出を実行します。
    IEnumerator OnGameOver()
    {
        // ゲームオーバーUIを表示
        gameOverUI.SetActive(true);
        // 2秒間待機
        yield return new WaitForSeconds(2);
        // 決定ボタンの入力を待機
        while (true)
        {
            // 決定ボタンが押された場合、待機ループを抜ける
            if (Input.GetButtonDown("Submit"))
            {
                break;
            }
            yield return null;
        }
        // アウトロ アニメーションへ切り替える
        animator.SetTrigger(outroId);
        // アニメーションが終了するまで1秒待機
        yield return new WaitForSeconds(1);
        // Stage 0シーンをリロードする
        SceneManager.LoadScene("Stage 0");
    }

    // このステージをステージクリアーとします。
    public void StageClear()
    {
        // ステージプレイ中のみ
        if (gameState == SceneState.Play)
        {
            gameState = SceneState.StageClear;
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
